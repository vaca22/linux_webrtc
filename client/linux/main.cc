/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "absl/flags/parse.h"
#include "api/scoped_refptr.h"
#include "conductor.h"
#include "flag_defs.h"
#include "main_wnd.h"
#include "peer_connection_client.h"
#include "rtc_base/physical_socket_server.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/thread.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:
    explicit CustomSocketServer(GtkMainWnd *wnd)
            : wnd_(wnd), conductor_(NULL), client_(NULL) {}

    virtual ~CustomSocketServer() {}

    void SetMessageQueue(rtc::Thread *queue) override { message_queue_ = queue; }

    void set_client(PeerConnectionClient *client) { client_ = client; }

    void set_conductor(Conductor *conductor) { conductor_ = conductor; }

    // Override so that we can also pump the GTK message loop.
    bool Wait(int cms, bool process_io) override {
        // Pump GTK events.
        // TODO(henrike): We really should move either the socket server or UI to a
        // different thread.  Alternatively we could look at merging the two loops
        // by implementing a dispatcher for the socket server and/or use
        // g_main_context_set_poll_func.
        while (gtk_events_pending())
            gtk_main_iteration();

        if (!wnd_->IsWindow() && !conductor_->connection_active() &&
            client_ != NULL && !client_->is_connected()) {
            message_queue_->Quit();
        }
        return rtc::PhysicalSocketServer::Wait(0 /*cms == -1 ? 1 : cms*/,
                                               process_io);
    }

protected:
    rtc::Thread *message_queue_;
    GtkMainWnd *wnd_;
    Conductor *conductor_;
    PeerConnectionClient *client_;
};

int main(int argc, char *argv[]) {
    PeerConnectionClient client;
    gtk_init(&argc, &argv);

    GtkMainWnd wnd("127.0.0.1", 9669,
                   false,
                   false);
    wnd.Create();

    CustomSocketServer socket_server(&wnd);


    auto conductor = rtc::make_ref_counted<Conductor>(&client, &wnd);
    socket_server.set_client(&client);
    socket_server.set_conductor(conductor);

    gtk_main();
    wnd.Destroy();

    while (gtk_events_pending()) {
        gtk_main_iteration();
    }

    return 0;
}
