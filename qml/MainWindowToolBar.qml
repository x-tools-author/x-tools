import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

ToolBar {
    id: root
    width: toolColumnLayout.width
    property int pageIndex: 0
    property int fixedpage: 0

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

    signal invokeAddPage(var qmlFile)
    signal invokeRemovePage(var pageIndex)

    ButtonGroup {
        id: bg
    }

    ColumnLayout {
        id: toolColumnLayout
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Item { Layout.minimumHeight: 24 }
        Repeater {
            id: btRepeater
            model: [
                ["qrc:/res/icon/IconSerialPort.svg",      EDToolsFactory.SerialportTool,      qsTr("SerialPort"),       "MainWindowSerialPortPage.qml"],
                ["qrc:/res/icon/IconBlueTooth.svg",       EDToolsFactory.SerialportTool,      qsTr("SerialPort"),       "MainWindowBlePage.qml"],
                ["qrc:/res/icon/IconUdpClient.svg",       EDToolsFactory.UdpClientTool,       qsTr("UDP Client"),       "MainWindowUdpClientPage.qml"],
                ["qrc:/res/icon/IconUdpServer.svg",       EDToolsFactory.UdpServerTool,       qsTr("UDP Server"),       "MainWindowUdpServerPage.qml"],
                ["qrc:/res/icon/IconTcpClient.svg",       EDToolsFactory.TcpClientTool,       qsTr("TCP Client"),       "MainWindowTcpClientPage.qml"],
                ["qrc:/res/icon/IconTcpServer.svg",       EDToolsFactory.TcpServerTool,       qsTr("TCP Server"),       "MainWindowTcpServerPage.qml"],
                ["qrc:/res/icon/IconWebScoketClient.svg", EDToolsFactory.WebSocketClientTool, qsTr("WebSocket Client"), "MainWindowWebSocketClientPage.qml"],
                ["qrc:/res/icon/IconWebSocketServer.svg", EDToolsFactory.WebSocketServerTool, qsTr("WebSocket Server"), "MainWindowWebSocketServerPage.qml"]
            ]
            EDToolButton {
                id: bt
                checkable: true
                icon.source: modelData[0]
                onClicked: root.pageIndex = index
                checked: pageIndex === index
                onDoubleClicked: invokeRemovePage(index)
                tips: modelData[2]
                Component.onCompleted: {
                    bg.addButton(bt)
                    if (index === pageIndex) {
                        checked = true
                    }
                }
            }
        }
        ToolButton {
            icon.source: "qrc:/res/icon/IconAdd.svg"
            onClicked: optionMenu.open()
            visible: false
            Menu {
                id: optionMenu
                Repeater {
                    model:  [
                        ["qrc:/res/icon/IconSerialPort.svg",      EDToolsFactory.SerialportTool,      qsTr("SerialPort"),       "MainWindowSerialPortPage.qml"],
                        ["qrc:/res/icon/IconUdpClient.svg",       EDToolsFactory.UdpClientTool,       qsTr("UDP Client"),       "MainWindowUdpClientPage.qml"],
                        ["qrc:/res/icon/IconUdpServer.svg",       EDToolsFactory.UdpServerTool,       qsTr("UDP Server"),       "MainWindowUdpServerPage.qml"],
                        ["qrc:/res/icon/IconTcpClient.svg",       EDToolsFactory.TcpClientTool,       qsTr("TCP Client"),       "MainWindowTcpClientPage.qml"],
                        ["qrc:/res/icon/IconTcpServer.svg",       EDToolsFactory.TcpServerTool,       qsTr("TCP Server"),       "MainWindowTcpServerPage.qml"],
                        ["qrc:/res/icon/IconWebScoketClient.svg", EDToolsFactory.WebSocketClientTool, qsTr("WebSocket Client"), "MainWindowWebSocketClientPage.qml"],
                        ["qrc:/res/icon/IconWebSocketServer.svg", EDToolsFactory.WebSocketServerTool, qsTr("WebSocket Server"), "MainWindowWebSocketServerPage.qml"]
                    ]
                    MenuItem {
                        text: modelData[2]
                        icon.source: modelData[0]
                        onTriggered: {
                            var temp = []
                            temp = btRepeater.model
                            temp.push(modelData)
                            btRepeater.model = temp
                            root.invokeAddPage(modelData[3])
                        }
                    }
                }
            }
        }
        Item { Layout.fillHeight: true }
        ToolButton {
            id: infoToolButton
            checkable: true
            icon.source: "qrc:/res/icon/IconInfo.svg"
            onClicked: pageIndex = btRepeater.model.length
            Component.onCompleted:  bg.addButton(infoToolButton)
        }
        ToolButton {
            id: settingsToolButton
            checkable: true
            icon.source: "qrc:/res/icon/IconSettings.svg"
            onClicked: pageIndex = btRepeater.model.length + 1
            Component.onCompleted: bg.addButton(settingsToolButton)
        }
        Item { Layout.minimumHeight: 24 }
    }

    Component.onCompleted: fixedpage = btRepeater.model.length

    function removePageIndex(index) {
        if (index >= fixedpage && index < btRepeater.model.length) {
            var newModel = btRepeater.model
            newModel.splice(index, 1)
            btRepeater.model = newModel
            pageIndex = pageIndex - 1
            console.info("Page index removed:", index)
        }
    }
}
