#ifndef SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_
#define SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_

#include "shell/browser/ui/native_view.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace electron {

enum class ScrollBarMode { kDisabled, kHiddenButEnabled, kEnabled };

#if defined(OS_MAC)
enum class ScrollElasticity {
  kAutomatic = 0,  // NSScrollElasticityAutomatic = 0
  kNone = 1,       // NSScrollElasticityNone = 1
  kAllowed = 2,    // NSScrollElasticityAllowed = 2
};
#endif

class NativeScrollView : public NativeView {
 public:
  NativeScrollView(absl::optional<ScrollBarMode> horizontal_mode,
                   absl::optional<ScrollBarMode> vertical_mode);

  // NativeView:
  void DetachChildView(NativeView* view) override;
  void TriggerBeforeunloadEvents() override;
#if defined(OS_MAC)
  void UpdateDraggableRegions() override;
#endif

  void SetContentView(scoped_refptr<NativeView> view);
  NativeView* GetContentView() const;

  void SetContentSize(const gfx::Size& size);
  gfx::Size GetContentSize() const;

#if defined(OS_MAC)
  void SetScrollPosition(gfx::Point point);
  gfx::Point GetScrollPosition() const;
  gfx::Point GetMaximumScrollPosition() const;

  void ScrollToPoint(gfx::Point point, const gin_helper::Dictionary& options);
  void ScrollPointToCenter(gfx::Point point,
                           const gin_helper::Dictionary& options);

  void SetOverlayScrollbar(bool overlay);
  bool IsOverlayScrollbar() const;
#endif

  void SetHorizontalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetHorizontalScrollBarMode() const;
  void SetVerticalScrollBarMode(ScrollBarMode mode);
  ScrollBarMode GetVerticalScrollBarMode() const;

#if defined(OS_MAC)
  void SetHorizontalScrollElasticity(ScrollElasticity elasticity);
  ScrollElasticity GetHorizontalScrollElasticity() const;
  void SetVerticalScrollElasticity(ScrollElasticity elasticity);
  ScrollElasticity GetVerticalScrollElasticity() const;

  void SetScrollEventsEnabled(bool enable);
  bool IsScrollEventsEnabled();
  void SetScrollWheelSwapped(bool swap);
  bool IsScrollWheelSwapped();
  void SetScrollWheelFactor(double factor);
  double GetScrollWheelFactor();
#endif

#if defined(TOOLKIT_VIEWS) && !defined(OS_MAC)
  void ClipHeightTo(int min_height, int max_height);
  int GetMinHeight() const;
  int GetMaxHeight() const;
  gfx::Rect GetVisibleRect() const;
  void SetAllowKeyboardScrolling(bool allow);
  bool GetAllowKeyboardScrolling() const;
  void SetDrawOverflowIndicator(bool indicator);
  bool GetDrawOverflowIndicator() const;
#endif

 protected:
  ~NativeScrollView() override;

  // NativeView:
  void SetWindowForChildren(NativeWindow* window) override;

  void InitScrollView(absl::optional<ScrollBarMode> horizontal_mode,
                      absl::optional<ScrollBarMode> vertical_mode);
  void SetContentViewImpl(NativeView* container);
  void DetachChildViewImpl();

 private:
  scoped_refptr<NativeView> content_view_;
};

}  // namespace electron

#endif  // SHELL_BROWSER_UI_NATIVE_SCROLL_VIEW_H_
