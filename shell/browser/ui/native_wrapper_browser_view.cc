#include "shell/browser/ui/native_wrapper_browser_view.h"

#include "content/public/browser/web_contents.h"
#include "shell/browser/api/electron_api_browser_view.h"
#include "shell/common/gin_helper/dictionary.h"

namespace electron {

NativeWrapperBrowserView::NativeWrapperBrowserView() {
  InitWrapperBrowserView();
}

NativeWrapperBrowserView::~NativeWrapperBrowserView() = default;

void NativeWrapperBrowserView::SetBrowserView(api::BrowserView* browser_view) {
  api_browser_view_ = browser_view;
  if (api_browser_view_)
    SetBrowserViewImpl();
}

void NativeWrapperBrowserView::SetBounds(
    const gfx::Rect& bounds,
    const BoundsAnimationOptions& options) {
  NativeView::SetBounds(bounds, options);
  if (api_browser_view_)
    api_browser_view_->view()->SetBounds(
        gfx::Rect(0, 0, bounds.width(), bounds.height()));
}

void NativeWrapperBrowserView::DetachBrowserView(NativeBrowserView* view) {
  if (!api_browser_view_ || api_browser_view_->view() != view)
    return;
  DetachBrowserViewImpl();
  api_browser_view_ = nullptr;
}

void NativeWrapperBrowserView::SetWindowForChildren(NativeWindow* window) {
  if (api_browser_view_)
    api_browser_view_->SetOwnerWindow(window);
}

}  // namespace electron
