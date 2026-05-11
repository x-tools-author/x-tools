---
description: "Apply Qt6 QML best practices when writing or modifying QML code"
---

# QML Best Practices

When writing or modifying QML code, follow these rules. These
apply to Qt 6 unless noted otherwise.

## Imports

- No version numbers on imports in Qt 6: `import QtQuick` not
  `import QtQuick 2.15`. Qt 5 code requires version numbers.
- No `import QtQuick.Window` when `import QtQuick` is present
- Use style-specific import when customizing controls:
  `import QtQuick.Controls.Basic` not plain
  `import QtQuick.Controls`. Qt 5: plain import is acceptable.

## Property bindings

- Prefer declarative bindings `prop: expr` over imperative `=`
- Imperative `=` permanently destroys the binding; use
  `Qt.binding(() => expr)` to restore if needed
- No function calls in hot bindings; cache in `readonly property`
- Use `Binding { when: ... }` to deactivate expensive bindings
- Use `Layout.*` for layout math; avoid `width: parent.width -
  sibling.width` patterns
- No circular dependencies: if A depends on B and B depends on A,
  one link must break

## Layouts

- Never mix `anchors` + `Layout.*` on the same item
- Size Layout children with `Layout.*` properties only: use
  `Layout.preferredWidth`, `Layout.fillWidth`, etc. Setting
  `width`/`height` directly breaks layout size negotiation.
  This applies at every nesting level.
- `anchors.fill: parent` over four separate anchor edges
- Do not anchor to `visible: false` items
- Do not anchor across unrelated visual tree branches
- Use `Row`/`Column` for static arrangements,
  `RowLayout`/`ColumnLayout` for resize-responsive UI

## Component loading

- Use `Loader` for conditional UI; set `active: false` when unused
- Guard `Loader.item` with `status === Loader.Ready`
- No `Qt.createComponent(url)` strings; use inline `Component {}`
- Set `Loader.asynchronous: true` for heavy components
- `Component.createObject()` only when parent is dynamic;
  otherwise prefer `Loader`

## ListView and delegates

- Use `required property` for model roles (type-safe, faster)
- Access roles as `model.roleName`; prevents shadowing
- Keep delegates minimal: complexity multiplies by item count
- `ListView.reuseItems: true` for large lists (Qt 6.7+); reset
  state in `onPooled`, restore in `onReused`
- No mutable JS variables in delegates; use QML properties
- `readonly property` for values computed at creation

## State management

- `states` for discrete configurations only, not continuous animations
- No `target` in `PropertyChanges` (Qt 6): use
  `PropertyChanges { someId.width: 100 }`. Qt 5: `target` is correct.
- Target transitions with `from`/`to` to avoid catch-all firing

## Images

- Always set `sourceSize` to prevent full-resolution decode
- `asynchronous: true` for network sources or large files
- Check `Image.status` for error handling

## Non-obvious pitfalls

- `parent` in delegates is the internal container, not the ListView;
  use `ListView.view` or an explicit id
- Dynamic scope is fragile: always use explicit id references for
  cross-component access
- `Timer.running` defaults to false; set `running: true` or call
  `.start()`
- `Connections` targets one object; use multiple blocks for multiple
  signal sources
