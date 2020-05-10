import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/page"
import "qrc:/qml/tools"
import "qrc:/qml/console"
import "qrc:/qml/settings"

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("瑞士军刀-V3版本正在开发中，请使用V2分支")
    flags: Qt.FramelessWindowHint | Qt.Dialog
    property var _currentIndex: 0

    Page {
        anchors.fill: parent
        background: Rectangle{color: "#222222"}
        header: MainWindowToolBar{
            title: mainWindow.title
        }
        contentItem: Item{
            MainWindowTabBar {
                id: tabBar
                anchors{left: parent.left;right: parent.right;top:parent.top}

                onCurrentIndexChanged: {
                    _currentIndex = currentIndex
                }
            }

            Item {
                id: pageViewer
                anchors{left: parent.left; right: parent.right; top: tabBar.bottom; bottom: parent.bottom}

                Repeater {
                    id: debugPageRepeater
                    model: [                                            // true表示页面为调试页面
                        ["qrc:/qml/SAKSerialportControl.qml",           true,  SAKDebugger.DebuggerTypeSerialport,  qsTr("串口调试")],
                        ["qrc:/qml/SAKUdpControl.qml",                  true,  SAKDebugger.DebuggerTypeUDP,         qsTr("UDP调试"), ],
                        ["qrc:/qml/SAKTcpClientControl.qml",            true,  SAKDebugger.DebuggerTypeTcpClient,   qsTr("TCP客户端")],
                        ["qrc:/qml/SAKTcpServerControl.qml",            true,  SAKDebugger.DebuggerTypeTcpServer,   qsTr("TCP服务器")],
                        ["",                                            false, -1,                                  " "],// 间隔符
                        ["qrc:/qml/tools/ToolsPage.qml",                false, -1,                                  qsTr("实用工具")],
                        ["qrc:/qml/settings/SettingsPage.qml",          false, -1,                                  qsTr("软件设置")],
                        ["qrc:/qml/console/ConsolePage.qml",            false, -1,                                  qsTr("终端输出")],
                    ]

                    Loader {
                        id: loader
                        anchors.fill: parent
                        visible: _currentIndex === index
                        Component.onCompleted: {
                            if (modelData[1]){
                                // 加载调试页面
                                var component = Qt.createComponent("qrc:/qml/page/DebugPage.qml")
                                sourceComponent = component
                            }else{
                                // 加载非调试页面
                                if (modelData[0]){
                                    component = Qt.createComponent(modelData[0])
                                    sourceComponent = component
                                }else{
                                    // 加载一个空页面
                                    sourceComponent = itemComponent
                                }
                            }
                        }

                        onLoaded: {
                            tabBar.push(modelData[3])
                            if (modelData[1]){
                                // 指定调试页面的设备控制面板
                                item.createControlPannel(modelData[0])
                            }
                        }
                    }
                }
            }// pageViewer
        }
    }

    Component {
        id: itemComponent
        Item{}
    }
}
