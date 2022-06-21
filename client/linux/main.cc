#include <glib.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "absl/flags/parse.h"
#include "api/scoped_refptr.h"
#include "conductor.h"
#include "flag_defs.h"
#include "main_wnd.h"
#include "rtc_base/physical_socket_server.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/thread.h"
#include "system_wrappers/include/field_trial.h"
#include "test/field_trial.h"


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkMainWnd wnd("127.0.0.1",
                   9669,
                   false,
                   false);
    wnd.Create();


    auto conductor = rtc::make_ref_counted<Conductor>( &wnd);

    gtk_main();
    wnd.Destroy();


    return 0;
}
