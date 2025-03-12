import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

Item {
    id: root
    width: toolColumnLayout.width
    Material.elevation: 4

    property int pageIndex: 0

    signal invokeAddPage(var qmlFile)
    signal invokeRemovePage(var pageIndex)

    CommonSeparator {
        edge: Qt.RightEdge
    }

    ButtonGroup {
        id: bg
    }

    ColumnLayout {
        id: toolColumnLayout
        spacing: 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Item {
            Layout.minimumHeight: 4
        }
        Repeater {
            id: btRepeater
            model: {
                var serialPort = [qsTr("Serial Port"), "qrc:/res/icons/serialPort.svg"];
                var bleCentral = [qsTr("BLE Central"), "qrc:/res/icons/bluetooth.svg"];
                var udpClient = [qsTr("UDP Client"), "qrc:/res/icons/udp-client.svg"];
                var ucpServer = [qsTr("UDP Server"), "qrc:/res/icons/udp-server.svg"];
                var tcpClient = [qsTr("TCP Client"), "qrc:/res/icons/tcp-client.svg"];
                var tcpServer = [qsTr("TCP Server"), "qrc:/res/icons/tcp-server.svg"];
                var wsClient = [qsTr("WebSocket Client"), "qrc:/res/icons/websocket-client.svg"];
                var wsServer = [qsTr("WebSocket Server"), "qrc:/res/icons/websocket-server.svg"];
                var tmp = [serialPort, bleCentral, udpClient, ucpServer, tcpClient, tcpServer, wsClient, wsServer];
                return tmp;
            }
            ToolButton {
                id: bt
                checkable: true
                icon.source: modelData[1]
                checked: pageIndex === index
                leftPadding: 0
                display: ToolButton.IconOnly
                onClicked: root.pageIndex = index
                onDoubleClicked: invokeRemovePage(index)
                Layout.fillWidth: true
                ToolTip.visible: hovered
                ToolTip.text: modelData[0]
                Component.onCompleted: {
                    bg.addButton(bt);
                    if (index === 1) {
                        visible = false;
                    }
                }
            }
        }
        Label {
            text: " "
            Layout.fillHeight: true
        }
        Repeater {
            id: infosSettingsRepeater
            model: {
                var tmp = [];
                tmp.push([qsTr("About"), "qrc:/res/icons/info.svg"]);
                tmp.push([qsTr("Settings"), "qrc:/res/icons/settings.svg"]);
                return tmp;
            }
            ToolButton {
                id: toolButton
                checkable: true
                icon.source: modelData[1]
                leftPadding: 0
                display: ToolButton.IconOnly
                onClicked: root.pageIndex = index + btRepeater.model.length
                Layout.fillWidth: true
                ToolTip.visible: hovered
                ToolTip.text: modelData[0]
                Component.onCompleted: bg.addButton(toolButton)
            }
        }
        Item {
            Layout.minimumHeight: 4
        }
    }

    Component.onCompleted: {
        var index = xSettings.value(settingKeys.pageIndex);
        if (index === btRepeater.model.length) {
            infosSettingsRepeater.itemAt(0).checked = true;
        } else if (index === (btRepeater.model.length + 1)) {
            infosSettingsRepeater.itemAt(1).checked = true;
        }
    }
}
