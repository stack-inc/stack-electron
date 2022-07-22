#include "shell/browser/ui/native_scroll_view.h"

#include "ui/base/ui_base_features.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/scroll_view.h"

#include "electron/shell/browser/ui/views/scroll_with_layers/scroll_view_scroll_with_layers.h"
#include "electron/shell/browser/ui/views/stack_smooth_scroll/stack_scroll_bar_views.h"

namespace electron {

namespace {

ScrollBarMode GetScrollBarMode(views::ScrollView::ScrollBarMode mode) {
  switch (mode) {
    case views::ScrollView::ScrollBarMode::kDisabled:
      return ScrollBarMode::kDisabled;
    case views::ScrollView::ScrollBarMode::kHiddenButEnabled:
      return ScrollBarMode::kHiddenButEnabled;
    case views::ScrollView::ScrollBarMode::kEnabled:
      return ScrollBarMode::kEnabled;
  }
}

void UpdateScrollBars(views::ScrollView* scroll_view, bool is_smooth_scroll) {
  if (!scroll_view)
    return;

  if (is_smooth_scroll) {
    scroll_view->SetHorizontalScrollBar(
        std::make_unique<StackScrollBarViews>(true));
    scroll_view->SetVerticalScrollBar(
        std::make_unique<StackScrollBarViews>(false));
  } else {
    scroll_view->SetHorizontalScrollBar(
        std::make_unique<views::ScrollBarViews>(true));
    scroll_view->SetVerticalScrollBar(
        std::make_unique<views::ScrollBarViews>(false));
  }
}

}  // namespace

void NativeScrollView::InitScrollView(
    absl::optional<ScrollBarMode> horizontal_mode,
    absl::optional<ScrollBarMode> vertical_mode) {
  views::ScrollView* scroll_view = nullptr;
  if (base::FeatureList::IsEnabled(::features::kUiCompositorScrollWithLayers)) {
    scroll_view = new ScrollViewScrollWithLayers();
  } else {
    scroll_view = new views::ScrollView();
  }

  scroll_view->SetBackgroundColor(absl::optional<SkColor>());

  on_contents_scrolled_subscription_ =
      scroll_view->AddContentsScrolledCallback(base::BindRepeating(
          &NativeScrollView::OnDidScroll, base::Unretained(this)));
  SetNativeView(scroll_view);

  if (horizontal_mode)
    SetHorizontalScrollBarMode(horizontal_mode.value());
  if (vertical_mode)
    SetVerticalScrollBarMode(vertical_mode.value());
}

void NativeScrollView::SetContentViewImpl(NativeView* view) {
  if (!GetNative() || !view)
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  view->set_delete_view(false);
  UpdateScrollBars(scroll, smooth_scroll_);
  auto content_view = std::unique_ptr<views::View>(view->GetNative());
  scroll->SetContents(std::move(content_view));
}

void NativeScrollView::DetachChildViewImpl() {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetContents(nullptr);
  content_view_->set_delete_view(true);
}

void NativeScrollView::SetContentSize(const gfx::Size& size) {
  if (!content_view_.get())
    return;
  content_view_->GetNative()->SetSize(size);
}

void NativeScrollView::SetScrollPosition(gfx::Point point) {
  if (!GetNative() || !content_view_.get())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  gfx::Size content_size = GetContentSize();
  gfx::Rect visible_rect = scroll->GetVisibleRect();
  int max_x_position = std::max(0, content_size.width() - visible_rect.width());
  int max_y_position =
      std::max(0, content_size.height() - visible_rect.height());
  gfx::Rect new_visible_rect(std::min(std::max(0, point.x()), max_x_position),
                             std::min(std::max(0, point.y()), max_y_position),
                             visible_rect.width(), visible_rect.height());
  content_view_->GetNative()->ScrollRectToVisible(new_visible_rect);
}

gfx::Point NativeScrollView::GetScrollPosition() const {
  if (!GetNative())
    return gfx::Point();
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  gfx::Rect visible_rect = scroll->GetVisibleRect();
  return gfx::Point(visible_rect.x(), visible_rect.y());
}

gfx::Point NativeScrollView::GetMaximumScrollPosition() const {
  if (!GetNative() || !content_view_.get())
    return gfx::Point();
  gfx::Size content_size = GetContentSize();
  gfx::Rect visible_rect = GetVisibleRect();
  return gfx::Point(std::max(0, content_size.width() - visible_rect.width()),
                    std::max(0, content_size.height() - visible_rect.height()));
}

void NativeScrollView::SetHorizontalScrollBarMode(ScrollBarMode mode) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  views::ScrollView::ScrollBarMode scroll_bar_mode =
      views::ScrollView::ScrollBarMode::kEnabled;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kDisabled;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kHiddenButEnabled;
      break;
    case ScrollBarMode::kEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kEnabled;
  }
  scroll->SetHorizontalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollView::GetHorizontalScrollBarMode() const {
  if (!GetNative())
    return ScrollBarMode::kDisabled;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return GetScrollBarMode(scroll->GetHorizontalScrollBarMode());
}

void NativeScrollView::SetVerticalScrollBarMode(ScrollBarMode mode) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  views::ScrollView::ScrollBarMode scroll_bar_mode =
      views::ScrollView::ScrollBarMode::kEnabled;
  switch (mode) {
    case ScrollBarMode::kDisabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kDisabled;
      break;
    case ScrollBarMode::kHiddenButEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kHiddenButEnabled;
      break;
    case ScrollBarMode::kEnabled:
      scroll_bar_mode = views::ScrollView::ScrollBarMode::kEnabled;
  }
  scroll->SetVerticalScrollBarMode(scroll_bar_mode);
}

ScrollBarMode NativeScrollView::GetVerticalScrollBarMode() const {
  if (!GetNative())
    return ScrollBarMode::kDisabled;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return GetScrollBarMode(scroll->GetVerticalScrollBarMode());
}

void NativeScrollView::SetScrollWheelSwapped(bool swap) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetTreatAllScrollEventsAsHorizontal(swap);
}

bool NativeScrollView::IsScrollWheelSwapped() {
  if (!GetNative())
    return false;

  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetTreatAllScrollEventsAsHorizontal();
}

void NativeScrollView::SetScrollEventsEnabled(bool enable) {
  scroll_events_ = enable;
}

bool NativeScrollView::IsScrollEventsEnabled() {
  return scroll_events_;
}

void NativeScrollView::SetHorizontalScrollElasticity(
    ScrollElasticity elasticity) {}

ScrollElasticity NativeScrollView::GetHorizontalScrollElasticity() const {
  return ScrollElasticity::kNone;
}

void NativeScrollView::SetVerticalScrollElasticity(
    ScrollElasticity elasticity) {}

ScrollElasticity NativeScrollView::GetVerticalScrollElasticity() const {
  return ScrollElasticity::kNone;
}

void NativeScrollView::ClipHeightTo(int min_height, int max_height) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->ClipHeightTo(min_height, max_height);
}

int NativeScrollView::GetMinHeight() const {
  if (!GetNative())
    return 0;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetMinHeight();
}

int NativeScrollView::GetMaxHeight() const {
  if (!GetNative())
    return 0;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetMaxHeight();
}

void NativeScrollView::ScrollRectToVisible(const gfx::Rect& rect) {
  if (!content_view_.get())
    return;
  content_view_->GetNative()->ScrollRectToVisible(rect);
}

gfx::Rect NativeScrollView::GetVisibleRect() const {
  if (!GetNative())
    return gfx::Rect();
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetVisibleRect();
}

void NativeScrollView::SetAllowKeyboardScrolling(bool allow) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetAllowKeyboardScrolling(allow);
}

bool NativeScrollView::GetAllowKeyboardScrolling() const {
  if (!GetNative())
    return false;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetAllowKeyboardScrolling();
}

void NativeScrollView::SetDrawOverflowIndicator(bool indicator) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetDrawOverflowIndicator(indicator);
}

bool NativeScrollView::GetDrawOverflowIndicator() const {
  if (!GetNative())
    return false;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  return scroll->GetDrawOverflowIndicator();
}

void NativeScrollView::SetSmoothScroll(bool enable) {
  if (smooth_scroll_ != enable && GetNative()) {
    auto* scroll = static_cast<views::ScrollView*>(GetNative());
    UpdateScrollBars(scroll, enable);
  }
  smooth_scroll_ = enable;
}

void NativeScrollView::OnDidScroll() {
  if (IsScrollEventsEnabled())
    NotifyDidScroll(this);
}

void NativeScrollView::SetBackgroundColor(SkColor color) {
  if (!GetNative())
    return;
  auto* scroll = static_cast<views::ScrollView*>(GetNative());
  scroll->SetBackgroundColor(color);
}

}  // namespace electron