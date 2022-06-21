// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_FOCUSGROUP_CONTROLLER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_FOCUSGROUP_CONTROLLER_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"

namespace blink {

class KeyboardEvent;
class Element;
class LocalFrame;

enum class FocusgroupDirection;

class CORE_EXPORT FocusgroupController {
  STATIC_ONLY(FocusgroupController);

 public:
  static bool HandleArrowKeyboardEvent(KeyboardEvent* event,
                                       const LocalFrame* frame);

 private:
  // Entry point into Focusgroup advancement. Returns true if the key press
  // moved the focus.
  static bool Advance(const Element* initial_element,
                      FocusgroupDirection direction);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_FOCUSGROUP_CONTROLLER_H_