#ifndef SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_
#define SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_

#include "shell/browser/api/electron_api_base_view.h"
#include "shell/browser/ui/native_scroll_view.h"

namespace electron {

namespace api {

class ScrollView : public BaseView {
 public:
  static gin_helper::WrappableBase* New(gin_helper::ErrorThrower thrower,
                                        gin::Arguments* args);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Local<v8::FunctionTemplate> prototype);

  // disable copy
  ScrollView(const ScrollView&) = delete;
  ScrollView& operator=(const ScrollView&) = delete;

 protected:
  ScrollView(gin::Arguments* args, NativeScrollView* scroll);
  ~ScrollView() override;

  // BaseView:
  void ResetChildView(BaseView* view) override;
  void ResetChildViews() override;

  void SetContentView(v8::Local<v8::Value> value);
  v8::Local<v8::Value> GetContentView() const;
  void SetContentSize(gfx::Size size);
  gfx::Size GetContentSize() const;
  void SetHorizontalScrollBarMode(std::string mode);
  std::string GetHorizontalScrollBarMode() const;
  void SetVerticalScrollBarMode(std::string mode);
  std::string GetVerticalScrollBarMode() const;
#if BUILDFLAG(IS_MAC)
  void SetHorizontalScrollElasticity(std::string elasticity);
  std::string GetHorizontalScrollElasticity() const;
  void SetVerticalScrollElasticity(std::string elasticity);
  std::string GetVerticalScrollElasticity() const;
  void SetScrollPosition(gfx::Point point);
  gfx::Point GetScrollPosition() const;
  gfx::Point GetMaximumScrollPosition() const;
  void SetOverlayScrollbar(bool overlay);
  bool IsOverlayScrollbar() const;
#endif
#if defined(TOOLKIT_VIEWS) && !BUILDFLAG(IS_MAC)
  void ClipHeightTo(int min_height, int max_height);
  int GetMinHeight() const;
  int GetMaxHeight() const;
  gfx::Rect GetVisibleRect() const;
  void SetAllowKeyboardScrolling(bool allow);
  bool GetAllowKeyboardScrolling() const;
  void SetDrawOverflowIndicator(bool indicator);
  bool GetDrawOverflowIndicator() const;
#endif

 private:
  NativeScrollView* scroll_;

  int32_t content_view_id_ = 0;
  v8::Global<v8::Value> content_view_;
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ELECTRON_API_SCROLL_VIEW_H_