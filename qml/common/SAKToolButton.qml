import QtQuick
import QtQuick.Controls

ToolButton {
    id: root

    property alias tips: tipLabel.text

    onHoveredChanged: {
        if (tips !== "") {
            if (hovered) {
                var popupX = mapToItem(tipsPopup.parent, 0, 0).x
                var popupY = mapToItem(tipsPopup.parent, 0, 0).y
                tipsPopup.x = popupX - (tipsPopup.width - root.width)/2
                tipsPopup.y = popupY - tipsPopup.height

                tipsPopup.width = tipLabel + tipsPopup.padding*2
                tipsPopup.open()
            } else {
                tipsPopup.close()
            }
        }
    }

    SAKPopup {
        id: tipsPopup
        dim: false
        modal: false
        contentItem: SAKLabel {
            id: tipLabel
            text: tips
        }
    }
}
