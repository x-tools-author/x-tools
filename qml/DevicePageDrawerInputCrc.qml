import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

EDGroupBox {
    id: root
    title: qsTr("CRC Calculator")

    property alias crcType: crcTypeComboBox.currentValue
    property alias crcStartIndex: startIndexSpinBox.value
    property alias crcEndIndex: endIndexSpinBox.value
    property alias crcEnable: appendCrcCheckbox.checked

    QtObject {
        id: settingKeys
        readonly property string inputCrcType: groupName + "/inputCrcType"
        readonly property string inputCrcStartIndex: groupName + "/inputCrcStartIndex"
        readonly property string inputCrcEndIndex: groupName + "/inputCrcEndIndex"
        readonly property string inputCrcAppendEnable: groupName + "/inputCrcAppendEnable"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent

        EDLabel {
            text: qsTr("CRC type")
        }
        EDCrcComboBox {
            id: crcTypeComboBox
            settingKey: settingKeys.inputCrcType
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Start index")
        }
        EDSpinBox {
            id: startIndexSpinBox
            settingKey: settingKeys.inputCrcStartIndex
            editable: true
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("End index(from tail)")
        }
        EDSpinBox {
            id: endIndexSpinBox
            settingKey: settingKeys.inputCrcEndIndex
            editable: true
            Layout.fillWidth: true
        }
        EDCheckBox {
            id: appendCrcCheckbox
            text: qsTr("Append CRC(before suffix)");
            settingKey: settingKeys.inputCrcAppendEnable
            Layout.columnSpan: 2
        }
        EDLabel {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("The start index is start from header, the first byte is 0. The end index is from tail, the last byte is 0. If parameters are set error, all bytes inputed will be calculated.")
        }
    }
}
