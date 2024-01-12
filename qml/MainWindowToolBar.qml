import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

ToolBar {
    id: root
    width: toolColumnLayout.width
    background: Rectangle {
        radius: isWindowed ? 8 : 0
        color: Material.primary
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: parent.color
            anchors.right: parent.right
            width: parent.radius
        }
    }

    property int pageIndex: 0

    signal invokeAddPage(var qmlFile)
    signal invokeRemovePage(var pageIndex)

    ButtonGroup {
        id: bg
    }

    ColumnLayout {
        id: toolColumnLayout
        spacing: 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Item {
            Layout.minimumHeight: titleBar.height
            Layout.fillWidth: true
            Row {
                spacing: 4
                anchors.centerIn: parent
                Text {
                    id: easyText
                    text: "Easy"
                    color: "red"
                    font.bold: true
                    font.pixelSize: 20
                    font.italic: true
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 1
                }
                Text {
                    text: "Debug"
                    color: "#efeff0"
                    font: easyText.font
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -1
                }
            }
            SAKHorizontalLine {
                anchors.bottom: parent.bottom
            }
        }
        Repeater {
            id: btRepeater
            model: {
                var serialPort = [qsTr("Serial Port"), "qrc:/resources/icon/IconSerialPort.svg"]


                /*var bleCentral = [qsTr("BLE Central"), "qrc:/resources/icon/IconBlueTooth.svg"]
                var udpClient = [qsTr("UDP Client"), "qrc:/resources/icon/IconUdpClient.svg"]
                var ucpServer = [qsTr("UDP Server"), "qrc:/resources/icon/IconUdpServer.svg"]
                var tcpClient = [qsTr("TCP Client"), "qrc:/resources/icon/IconTcpClient.svg"]
                var tcpServer = [qsTr("TCP Server"), "qrc:/resources/icon/IconTcpServer.svg"]
                var wsClient = [qsTr("WebSocket Client"), "qrc:/resources/icon/IconWebSocketClient.svg"]
                var wsServer = [qsTr("WebSocket Server"), "qrc:/resources/icon/IconWebSocketServer.svg"]*/
                var tmp = [serialPort /*, bleCentral, udpClient, ucpServer, tcpClient, tcpServer, wsClient, wsServer*/
                        ]
                return tmp
            }
            MainWindowToolBarItem {
                id: bt
                checkable: true
                icon.source: modelData[1]
                checked: pageIndex === index
                text: modelData[0]
                leftPadding: 4
                onClicked: root.pageIndex = index
                onDoubleClicked: invokeRemovePage(index)

                Layout.fillWidth: true
                Component.onCompleted: {
                    bg.addButton(bt)
                    if (index === pageIndex) {
                        checked = true
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            SAKHorizontalLine {
                anchors.bottom: parent.bottom
            }
        }
        MainWindowToolBarItem {
            id: aboutToolButton
            checkable: true
            text: qsTr("About")
            icon.source: "qrc:/resources/icon/IconInfo.svg"
            onClicked: pageIndex = btRepeater.model.length
            Layout.fillWidth: true
            Component.onCompleted: bg.addButton(aboutToolButton)
        }
        MainWindowToolBarItem {
            id: settingsToolButton
            checkable: true
            text: qsTr("Settings")
            icon.source: "qrc:/resources/icon/IconSettings.svg"
            onClicked: pageIndex = btRepeater.model.length + 1
            Layout.fillWidth: true
            Component.onCompleted: bg.addButton(settingsToolButton)
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
