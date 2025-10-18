import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../controls"
import "../../common"

Pane {
    id: root
    padding: 0
    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        TabBar {
            Layout.fillWidth: true
            Repeater {
                model: ["RS232", "BLE", "TCP-C", "TCP-S", "UDP-C", "UDP-S", "WS-C", "WS-S", "Multicast", "Broadcast", "LocalSocket-C", "LocalSocket-S", "ChartsTest"]
                TabButton {
                    text: modelData
                }
            }
        }
        CommonSeparator {
            height: 1
            Layout.fillWidth: true
        }
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
