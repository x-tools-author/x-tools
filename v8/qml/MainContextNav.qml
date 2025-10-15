import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "controls"

Pane {
    id: root
    padding: 0
    Material.elevation: 4
    Component.onCompleted: {
        width = navLayout.width;
    }

    property int navIndex: 0

    ButtonGroup {
        id: btnGroup
    }

    ColumnLayout {
        id: navLayout
        spacing: 0
        anchors.fill: parent
        Repeater {
            id: navRepeater1
            model: {
                var tmp = [];
                tmp.push(["Common Links", "qrc:/v8/res/icons/send.svg"]);
                tmp.push(["Modbus Assistant", "qrc:/v8/res/icons/send.svg"]);
                tmp.push(["MQTT Assistant", "qrc:/v8/res/icons/send.svg"]);
                tmp.push(["CAN-Bus Assistant", "qrc:/v8/res/icons/send.svg"]);
                tmp.push(["Data Flow", "qrc:/v8/res/icons/graph_1.svg"]);
                return tmp;
            }
            ControlNavToolButton {
                id: btn1
                checkable: true
                icon.source: modelData[1]
                text: modelData[0]
                onClicked: root.navIndex = index
                ToolTip.text: modelData[0]
                ToolTip.visible: false
                Layout.fillWidth: true
                Component.onCompleted: {
                    btnGroup.addButton(btn1);
                    if (index === root.navIndex) {
                        btn1.checked = true;
                    }
                }
            }
        }
        ControlSeparator {
            height: 1
            Layout.fillWidth: true
        }
        Label {
            Layout.fillHeight: true
        }
        Repeater {
            id: navRepeater2
            model: {
                var tmp = [];
                tmp.push([qsTr("Tool Kits"), "qrc:/v8/res/icons/construction.svg"]);
                tmp.push([qsTr("Log View"), "qrc:/v8/res/icons/bug_report.svg"]);
                tmp.push([qsTr("Settings"), "qrc:/v8/res/icons/settings.svg"]);
                tmp.push([qsTr("About"), "qrc:/v8/res/icons/info.svg"]);
                return tmp;
            }
            ControlNavToolButton {
                id: btn2
                checkable: true
                icon.source: modelData[1]
                text: modelData[0]
                ToolTip.text: modelData[0]
                ToolTip.visible: false
                Layout.fillWidth: true
                Component.onCompleted: {
                    btnGroup.addButton(btn2);
                    if (index + navRepeater1.count === root.navIndex) {
                        btn2.checked = true;
                    }
                }
                onClicked: {
                    root.navIndex = index + navRepeater1.count;
                }
            }
        }
    }

    function setChecked(index) {
        if (index < navRepeater1.count) {
            navRepeater1.itemAt(index).checked = true;
        } else {
            index = index - navRepeater1.count;
            navRepeater2.itemAt(index).checked = true;
        }
    }
}
