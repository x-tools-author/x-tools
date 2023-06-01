import QtQuick
import QtQuick.Controls

Popup {
    id: root
    dim: true
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    modal: true

    signal accepted()
    signal rejected()
}
