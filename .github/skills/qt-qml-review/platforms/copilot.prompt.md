---
description: "Review Qt6 QML code for correctness, best practices, and performance"
---

# Qt QML Code Review

Review QML code for the following rules. Report only confirmed
findings with file path and line number.

## Imports (IMP)

- No `import QtQuick.Window` when `QtQuick` is imported (IMP-1)
- No versioned imports in Qt 6: `import QtQuick` not
  `import QtQuick 2.15` (IMP-2)
- Use style-specific import when customizing controls:
  `QtQuick.Controls.Basic` not plain `QtQuick.Controls` (IMP-3)
- Sort imports: Qt modules, third-party, local C++, QML folders (IMP-4)

## Attribute ordering (ORD)

- Order within each object: (1) id, (2) property declarations,
  (3) signal declarations, (4) property assignments, (5) attached
  properties, (6) states, (7) transitions, (8) signal handlers,
  (9) child objects, (10) JavaScript functions (ORD-1)

## Bindings and properties (BND)

- Use typed properties, not `property var` (BND-1)
- Imperative `=` destroys bindings; use `Qt.binding()` to restore (BND-2)
- Watch for multi-cycle binding loops: A changes B via handler,
  B's binding updates A (agent)
- Missing `readonly` on properties that are bound but never
  imperatively assigned (agent)
- `pragma ComponentBehavior: Bound` on files with delegates that
  access outer-scope ids (agent)

## Layout and anchoring (LAY)

- Never mix `anchors` with `Layout.*` on the same item (LAY-1)
- No bare width/height on Layout children; use `Layout.*`
  properties (LAY-2)
- `anchors.fill: parent` over four separate edges (LAY-3)
- Do not anchor to items with `visible: false` (LAY-4)
- No `parent.parent` in anchor targets; use explicit id (LAY-5)

## Loader and lifecycle (LDR)

- Guard `Loader.item` with `status === Loader.Ready` (LDR-1)
- No `Qt.createComponent()` with string URL (LDR-2)
- No `Qt.createQmlObject()` (LDR-3)
- Track objects from createObject() and destroy them (LDR-4)
- No Loader with both source and sourceComponent (LDR-5)

## ListView and delegates (DEL)

- Use `required property` for model roles (DEL-1)
- No `var` in delegates with reuseItems (DEL-2)
- No `connect()` in Component.onCompleted; use Connections (DEL-3)
- Component.onCompleted does not re-fire on reuse; use
  ListView.onReused (DEL-4)
- When using required property, also declare
  `required property int index` if index is accessed (DEL-5)

## States and transitions (STA)

- Qt 6 PropertyChanges: use `id.prop: val` not `target: id` (STA-1)
- Transitions need explicit from/to (STA-2)
- No imperative `=` inside PropertyChanges; use `:` binding (STA-4)

## Signals and connections (SIG)

- Connections must have explicit `target` (SIG-1)
- Use `function onFoo() {}` not `onFoo:` handler syntax (SIG-2)
- Do not mix old and new handler syntax in same Connections
  block — function handlers are silently ignored (SIG-3)

## Images (IMG)

- Always set `sourceSize` (IMG-1)
- `asynchronous: true` for network sources (IMG-2)
- Check Image.status for error handling on dynamic sources (IMG-3)

## Performance (PRF)

- Use `Item` not `Rectangle { color: "transparent" }` for
  grouping — transparent Rectangle still creates geometry node (PRF-1)
- `visible: false` not `opacity: 0` when not animating (PRF-2)
- `textFormat: Text.PlainText` when rich text not needed (PRF-5)
- Cache expensive binding expressions in `readonly property` (PRF-7)

## JavaScript (JS)

- `let`/`const` not `var` (JS-1)
- Strict equality `===`/`!==` not `==`/`!=` (JS-2)

## Error handling (ERR)

- Use `https://` not `http://` (ERR-1)
- No hardcoded Unix paths like `/tmp/` (ERR-2)
