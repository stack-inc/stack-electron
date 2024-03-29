# ContainerView

A `ContainerView` can be used to embed additional views hierarchy into a
[`BrowserWindow`](browser-window.md).
It extends [`BaseView`](base-view.md).

## Class: ContainerView extends `BaseView`

> Create and control hierarchy of views.

Process: [Main](../glossary.md#main-process)

### `new ContainerView([options])` _Experimental_

* `options` Object (optional)
  * `vibrant` boolean (optional) - Enables the vibrant visual effect. Default is `false`.
  * `blurred` boolean (optional) - Enables the blurred visual effect. Default is `false`.

Creates the new container view.

### Static Methods

The `ContainerView` class has the following static methods:

#### `ContainerView.getAllViews()`

Returns `ContainerView[]` - An array of all created container views.

#### `ContainerView.fromId(id)`

* `id` Integer

Returns `ContainerView | null` - The container view with the given `id`.

### Instance Methods

Objects created with `new ContainerView` have the following instance methods:

#### `view.addChildView(view)` _Experimental_

* `view` [BaseView](base-view.md)

#### `view.removeChildView(view)` _Experimental_

* `view` [BaseView](base-view.md)

#### `view.rearrangeChildViews()`

Rearranges child views according to their z-indexes.

#### `view.getViews()` _Experimental_

Returns `BaseView[]` - an array of all BaseViews that have been attached
with `addChildView`.
