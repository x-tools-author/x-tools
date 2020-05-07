import QtQuick 2.12
import QtQuick.Layouts 1.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

GridLayout {
    id: root
    columns: 2
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    property SAKDebugger sakdebugger: null

    SAKText {
        text: qsTr("串口选择")
        height: 25
        verticalAlignment: Text.AlignVCenter
    }

    SAKComboBox {
        model: ["COM1", "COM2"]
        Layout.fillWidth: true
    }

    SAKCheckBox {
        text: qsTr("自定义波特率")
        Layout.columnSpan: 2
    }

    Repeater {
        model: [qsTr("波特率"), qsTr("数据位"), qsTr("停止位"), qsTr("奇偶位")]

        SAKText {
            text: modelData
            Layout.column: 0
            Layout.row: index+2
        }
    }

    Repeater {
        model: 4
        SAKComboBox {
            model: ["9600", "115200"]
            Layout.column: 1
            Layout.row: index+2
            Layout.fillWidth: true
        }
    }

    Repeater {
        model: [qsTr("刷新设备"), qsTr("打开设备")]

        SAKButton {
            text: modelData
            Layout.fillWidth: true
        }
    }
}
