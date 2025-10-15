import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../controls"

Pane {
    id: root
    spacing: 0

    property int navIndex: 0
    property alias model: navRepeater.model

    ButtonGroup {
        id: btnGroup
    }

    ColumnLayout {
        id: navLayout
        spacing: 0
        anchors.fill: parent
        Repeater {
            id: navRepeater
            ControlNavToolButton {
                id: ctrlBtn
                text: modelData
                checkable: true
                display: ToolButton.TextOnly
                Layout.fillWidth: true
                onClicked: {
                    root.navIndex = index;
                }
                Component.onCompleted: {
                    btnGroup.addButton(ctrlBtn);
                    if (index === root.navIndex) {
                        ctrlBtn.checked = true;
                    }
                }
            }
        }
        Label {
            text: " "
            Layout.fillHeight: true
        }
    }

    Component.onCompleted: {
        width = navLayout.width;
    }
}
