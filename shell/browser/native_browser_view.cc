// Copyright (c) 2017 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/native_browser_view.h"

#include "shell/browser/api/electron_api_web_contents.h"
#include "shell/browser/ui/inspectable_web_contents.h"
#include "shell/browser/ui/inspectable_web_contents_view.h"
#include "ui/gfx/image/image.h"

namespace electron {

NativeBrowserView::NativeBrowserView(
    InspectableWebContents* inspectable_web_contents)
    : inspectable_web_contents_(inspectable_web_contents) {
  Observe(inspectable_web_contents_->GetWebContents());
}

NativeBrowserView::~NativeBrowserView() = default;

InspectableWebContentsView* NativeBrowserView::GetInspectableWebContentsView() {
  if (!inspectable_web_contents_)
    return nullptr;
  return inspectable_web_contents_->GetView();
}

void NativeBrowserView::ShowThumbnail(gfx::Image thumbnail) {
  InspectableWebContentsView* iwc_view = GetInspectableWebContentsView();
  if (!iwc_view)
    return;
  iwc_view->ShowThumbnail(thumbnail);
}

void NativeBrowserView::HideThumbnail() {
  InspectableWebContentsView* iwc_view = GetInspectableWebContentsView();
  if (!iwc_view)
    return;
  iwc_view->HideThumbnail();
}

void NativeBrowserView::WebContentsDestroyed() {
  inspectable_web_contents_ = nullptr;
}

}  // namespace electron
