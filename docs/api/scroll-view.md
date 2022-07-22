# ScrollView

Show a part of view with scrollbar.
The `ScrollView` can show an arbitrary content view inside it. It is used to make
any View scrollable. When the content is larger than the `ScrollView`,
scrollbars will be optionally showed. When the content view is smaller
then the `ScrollView`, the content view will be resized to the size of the
`ScrollView`.
The scrollview supports keyboard UI and mousewheel.
It extends [`BaseView`](base-view.md).

## Class: ScrollView extends `BaseView`

> Create and control scroll views.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const path = require("path");
const { app, BrowserView, BaseWindow, ContainerView, ScrollView, WrapperBrowserView } = require("electron");

const WEBVIEW_WIDTH = 600;
const GAP = 30;
const URLS = [
  "https://theverge.com/",
  "https://mashable.com",
  "https://www.businessinsider.com",
  "https://wired.com",
  "https://macrumors.com",
  "https://gizmodo.com",
  "https://maps.google.com/",
  "https://sheets.google.com/",
];

global.win = null;

app.whenReady().then(() => {
  win = new BaseWindow({ autoHideMenuBar: true, width: 1400, height: 700 });

  // The content view.
  const contentView = new ContainerView();
  contentView.setBackgroundColor("#1F2937");
  win.setContentBaseView(contentView);
  contentView.setBounds({x: 0, y: 0, width: 1378, height: 600});

  // Scroll
  const scroll = new ScrollView({ smoothScroll: true });
  scroll.setHorizontalScrollBarMode("enabled");
  scroll.setVerticalScrollBarMode("disabled");
  contentView.addChildView(scroll);
  scroll.setBounds({x: 0, y: 0, width: 1377, height: 600});

  // Scroll content
  const scrollContent = new ContainerView();
  scroll.setContentView(scrollContent);
  scrollContent.setBounds({x: 0, y: 0, width: URLS.length * (WEBVIEW_WIDTH + GAP), height: 600});

  for (var i = 1; i <= URLS.length; i++) {
    const browserView = new BrowserView();
    browserView.setBackgroundColor("#ffffff");
    const wrapperBrowserView = new WrapperBrowserView({ 'browserView': browserView });
    const webContentView = new ContainerView();
    webContentView.addChildView(wrapperBrowserView);
    wrapperBrowserView.setBounds({x: 0, y: 0, width: 600, height: 540});
    scrollContent.addChildView(webContentView);
    webContentView.setBounds({x: i*(WEBVIEW_WIDTH + GAP)+GAP, y: 30, width: 600, height: 540});
    browserView.webContents.loadURL(URLS[i-1]);
  }
});
```

### `new ScrollView([options])` _Experimental_

* `options` Object (optional)
  * `horizontalScrollBarMode` string (optional) - Can be `disabled`, `enabled-but-hidden`, `enabled`. Default is `enabled`.
  * `verticalScrollBarMode` string (optional) - Can be `disabled`, `enabled-but-hidden`, `enabled`. Default is `enabled`.
  * `smoothScroll` boolean (optional) - When is `true` enables smooth scroll in ScrollView. Default is `false`.

Creates the new scroll view.

### Instance Events

Objects created with `new ScrollView` emit the following events:

#### Event: 'did-scroll' _Experimental_

Returns:

* `event` Event

Emitted when the content view is being scrolled.

#### Event: 'will-start-live-scroll' _macOS_ _Experimental_

Returns:

* `event` Event

Emitted at the beginning of user-initiated live scroll tracking (gesture scroll or scroller tracking, for example, thumb dragging).

#### Event: 'did-live-scroll' _macOS_ _Experimental_

Returns:

* `event` Event

Emitted after changing the clipview bounds origin due to a user-initiated event.

#### Event: 'did-end-live-scroll' _macOS_ _Experimental_

Returns:

* `event` Event

Emitted at the end of live scroll tracking.

#### Event: 'scroll-wheel' _macOS_ _Experimental_

Returns:

* `event` Event
* `mouseEvent` boolean - Set to `true` if generated by the mouse.
* `scrollingDeltaX` Float - The scroll wheel’s horizontal delta.
* `scrollingDeltaY` Float - The scroll wheel’s vertical delta.
* `phase` string - The phase of a gesture event.
* `momentumPhase` string - The momentum phase for a scroll or flick gesture.

Emitted when the mouse’s scroll wheel has moved.

 Both `phase` and `momentumPhase` can be one of the following values:
* `none` - The event is not associated with a phase.
* `began` - An event phase has begun.
* `stationary` - An event phase is in progress but hasn't moved since the previous event.
* `changed` - An event phase has changed.
* `ended` - The event phase ended.
* `cancelled` - The system canceled the event phase.
* `mayBegin` - The system event phase may begin.

A gesture phase corresponds to a fluid gesture event. As a gesture event occurs,
its phase begins with `began` and ends with either `ended` or `canceled`.
All the gesture events are sent to the view under the cursor when the `began` occurred.
Technically, a gesture scroll event starts with a `began` phase and ends with a
`ended`. However, when the user puts two fingers down on a trackpad, the
trackpad issues `mayBegin`, followed by `began`, `cancelled`, or `ended`.
The `mayBegin` event phase signals that scrolling is about to begin before the
gesture has technically started. A Magic Mouse does not issue `mayBegin` scroll
wheel events.
Legacy scroll wheel events (say from a Mighty Mouse) and momentum scroll wheel
events both have a phase of `none`. (Legacy scroll wheel events also have
a `momentumPhase` of `none`.)

The `momentumPhase` helps you detect momentum scrolling, in which the hardware
continues to issue scroll wheel events even though the user is no longer
physically scrolling. Devices such as Magic Mouse and Multi-Touch trackpad
enable momentum scrolling.
During non momentum scrolling, each `scroll-wheel` event is routed to the view
that is beneath the pointer for that event. In non momentum `scroll-wheel`
events, `momentumPhase` has the value `none`.
During momentum scrolling, each `scroll-wheel` event is routed to the view
that was beneath the pointer when momentum scrolling started. In momentum
`scroll-wheel` events, phase has the value `none`. When the device switches
from user-performed scroll events to momentum scroll wheel events,
`momentumPhase` is set to `began`. For subsequent momentum scroll wheel events,
`momentumPhase` is set to `changed` until the momentum subsides, or the user
stops the momentum scrolling; the final momentum scroll wheel event has a
`momentumPhase` value of `ended`.

It’s important to understand how the difference in the event-routing behavior
can affect the events you receive, depending on the scrolling device users are
using. When using a Multi-Touch trackpad, users must physically stop scrolling
before they can move the pointer, so you won’t receive any mouse-movement
events during a scroll. In contrast, a Magic Mouse lets users move
the pointer and click in the middle of a scroll. Because a scroll doesn’t end
until the user lifts their finger, any additional movement by a finger resting
on a Magic Mouse is still considered to be part of the original scroll, which
means that the event’s phase property is `changed` and not `began`, as you
might expect. If you process `scroll-wheel` events directly, be prepared to
receive mouse-movement events (such as `leftMouseDown`, `rightMouseDragged`,
and `otherMouseUp`) between `began` and `ended` when a Magic Mouse is in use.

### Static Methods

The `ScrollView` class has the following static methods:

#### `ScrollView.getAllViews()`

Returns `ScrollView[]` - An array of all created scroll views.

#### `ScrollView.fromId(id)`

* `id` Integer

Returns `ScrollView | null` - The scroll view with the given `id`.

### Instance Methods

Objects created with `new ScrollView` have the following instance methods:

#### `view.setContentView(contents)` _Experimental_

* `contents` [BaseView](base-view.md)

Set the contents. The contents is the view that needs to scroll.

  #### `view.getContentView()` _Experimental_

Returns [`BaseView`](base-view.md) - The contents of the `view`.

#### `view.setContentSize(size)` _Experimental_

* `size` [Size](structures/size.md)

Set the size of the contents.

#### `view.getContentSize()` _Experimental_

Returns [`Size`](structures/size.md) - The `size` of the contents.

#### `view.setHorizontalScrollBarMode(mode)` _Experimental_

* `mode` string - Can be `disabled`, `enabled-but-hidden`, `enabled`. Default is `enabled`.

Controls how the horizontal scroll bar appears and functions.
* `disabled` - The scrollbar is hidden, and the pane will not respond to e.g. mousewheel events even if the contents are larger than the viewport.
* `enabled-but-hidden` - The scrollbar is hidden whether or not the contents are larger than the viewport, but the pane will respond to scroll events.
*`enabled` - The scrollbar will be visible if the contents are larger than the viewport and the pane will respond to scroll events.

#### `view.getHorizontalScrollBarMode()` _Experimental_

Returns `string` - horizontal scrollbar mode.

#### `view.setVerticalScrollBarMode(mode)` _Experimental_

* `mode` string - Can be `disabled`, `enabled-but-hidden`, `enabled`. Default is `enabled`.

Controls how the vertical scroll bar appears and functions.
* `disabled` - The scrollbar is hidden, and the pane will not respond to e.g. mousewheel events even if the contents are larger than the viewport.
* `enabled-but-hidden` - The scrollbar is hidden whether or not the contents are larger than the viewport, but the pane will respond to scroll events.
*`enabled` - The scrollbar will be visible if the contents are larger than the viewport and the pane will respond to scroll events.

#### `view.getVerticalScrollBarMode()` _Experimental_

Returns `string` - vertical scrollbar mode.

#### `view.setHorizontalScrollElasticity(elasticity)` _Experimental_

* `elasticity` string - Can be `automatic`, `none`, `allowed`. Default is `automatic`.

The scroll view’s horizontal scrolling elasticity mode.
A scroll view can scroll its contents past its bounds to achieve an elastic effect.
When set to `automatic`, scrolling the horizontal axis beyond its document
bounds only occurs if the document width is greater than the view width,
or the vertical scroller is hidden and the horizontal scroller is visible.
* `automatic` - Automatically determine whether to allow elasticity on this axis.
* `none` - Disallow scrolling beyond document bounds on this axis.
* `allowed` - Allow content to be scrolled past its bounds on this axis in an elastic fashion.

#### `view.getHorizontalScrollElasticity()` _Experimental_

Returns `string` - The scroll view’s horizontal scrolling elasticity mode.

#### `view.setVerticalScrollElasticity(elasticity)` _Experimental_

* `elasticity` string - Can be `automatic`, `none`, `allowed`. Default is `automatic`.

The scroll view’s vertical scrolling elasticity mode.

#### `view.getVerticalScrollElasticity()` _Experimental_

Returns `string` - The scroll view’s vertical scrolling elasticity mode.

#### `view.setScrollPosition(point)` _Experimental_

* `point` [Point](structures/point.md) - The point in the `contentView` to scroll to.

Scroll to the horizontal (`point.x`) and vertical (`point.y`) position.

#### `view.getScrollPosition()` _Experimental_

Returns [`Point`](structures/point.md) - The horizontal and vertical scroll position.

#### `view.getMaximumScrollPosition()` _Experimental_

Returns [`Point`](structures/point.md) - The maximum horizontal and vertical scroll position.

#### `view.setOverlayScrollbar(overlay)` _macOS_ _Experimental_

* `overlay` boolean - Sets the scroller style used by the scroll view.

#### `view.isOverlayScrollbar()` _macOS_ _Experimental_

Returns boolean - The scroller style used by the scroll view.

#### `view.setScrollEventsEnabled(enable)` _Experimental_

* `enable` boolean - Whether the scroll events are enabled. Default is `false`.

#### `view.isScrollEventsEnabled()` _Experimental_

Returns `boolean` - Whether the scroll events are enabled.

#### `view.setScrollWheelSwapped(swap)` _Experimental_

* `swap` boolean - Whether the mouse wheel should scroll horizontally. Default is `false`.

Swaps the behaviour of mouse wheel.

#### `view.isScrollWheelSwapped()` _Experimental_

Returns `boolean` - Whether the mouse wheel scrolls horizontally.

#### `view.setScrollWheelFactor(factor)` _macOS_ _Experimental_

* `factor` Double - Scrolling factor used for mouse wheel. Default is 1.0.

#### `view.getScrollWheelFactor()` _macOS_ _Experimental_

Returns `Double` - Scrolling factor used for mouse wheel.

#### `view.clipHeightTo(minHeight, maxHeight)` _Windows_ _Experimental_

* `minHeight` Integer - The min height for the bounded scroll view.
* `maxHeight` Integer - The max height for the bounded scroll view.

Turns this scroll view into a bounded scroll view, with a fixed height.
By default, a ScrollView will stretch to fill its outer container.

#### `view.getMinHeight()` _Windows_ _Experimental_

Returns `Integer` - The min height for the bounded scroll view.

This is negative value if the view is not bounded.

#### `view.getMaxHeight()` _Windows_ _Experimental_

Returns `Integer` - The max height for the bounded scroll view.

This is negative value if the view is not bounded.

#### `view.scrollRectToVisible(rect)` _Windows_ _Experimental_

* `rect` [`Rectangle`](structures/rectangle.md) - The region in the `contentView` to scroll to.

Scrolls the specified region, in this View's coordinate system, to be visible.

#### `view.getVisibleRect()` _Windows_ _Experimental_

Returns [`Rectangle`](structures/rectangle.md) - The visible region of the `contentView`.

#### `view.setAllowKeyboardScrolling(allow)` _Windows_ _Experimental_

* `allow` boolean - Whether the left/right/up/down arrow keys attempt to scroll the view.

#### `view.getAllowKeyboardScrolling()` _Windows_ _Experimental_

Returns `boolean` - Gets whether the keyboard arrow keys attempt to scroll the view. Default is `true`.

#### `view.SetDrawOverflowIndicator(indicator)` _Windows_ _Experimental_

* `indicator` boolean - Whether to draw a white separator on the four sides of the scroll view when it overflows.

#### `view.GetDrawOverflowIndicator()` _Windows_ _Experimental_

Returns `boolean` - Gets whether to draw a white separator on the four sides of the scroll view when it overflows. Default is `true`.