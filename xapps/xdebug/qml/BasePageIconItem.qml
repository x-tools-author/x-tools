import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

Row {
    id: root
    spacing: 4

    property alias iconSource: toolButton.icon.source
    property alias valueName: nameLabel.text
    property bool menuShowOnTop: true
    property bool isActionGroup: false
    property var menuItemContext: []
    property var value: null
    property int defaultIndex: 0
    property string toolTipText: ""

    signal hasBeenTriggered

    ToolButton {
        id: toolButton
        display: ToolButton.IconOnly
        anchors.verticalCenter: parent.verticalCenter
        ToolTip.visible: hovered
        ToolTip.text: root.toolTipText
        onClicked: {
            toolButtonMenu.x = (toolButton.width - toolButtonMenu.width) / 2
            if (menuShowOnTop) {
                toolButtonMenu.y = 0 - toolButtonMenu.height
            }
            toolButtonMenu.open()
        }
    }
    Label {
        id: nameLabel
        anchors.verticalCenter: parent.verticalCenter
    }

    ButtonGroup {
        id: buttonGroup
    }

    Menu {
        id: toolButtonMenu
        Repeater {
            model: menuItemContext
            MenuItem {
                id: menuItem
                checkable: true
                text: modelData[0]
                onCheckedChanged: {
                    if (checked) {
                        nameLabel.text = modelData[0]
                    }
                }
                onTriggered: {
                    root.value = modelData[1]
                    root.hasBeenTriggered()
                }

                property var data: modelData[1]

                Component.onCompleted: {
                    if (isActionGroup) {
                        buttonGroup.addButton(menuItem)
                        if (index === defaultIndex) {
                            menuItem.checked = true
                        }
                    }
                }
            }
        }
    }

    function setValue(v) {
        root.value = v
        for (var i = 0; i < toolButtonMenu.count; i++) {
            var item = toolButtonMenu.itemAt(i)
            if (Number(item.data) === Number(v)) {
                item.checked = true
                break
            }
        }
    }
}
