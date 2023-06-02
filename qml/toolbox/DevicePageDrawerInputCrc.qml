import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

SAKGroupBox {
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

        SAKLabel {
            text: qsTr("CRC type")
        }
        SAKCrcComboBox {
            id: crcTypeComboBox
            settingKey: settingKeys.inputCrcType
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("Start index")
        }
        SAKSpinBox {
            id: startIndexSpinBox
            settingKey: settingKeys.inputCrcStartIndex
            editable: true
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("End index(from tail)")
        }
        SAKSpinBox {
            id: endIndexSpinBox
            settingKey: settingKeys.inputCrcEndIndex
            editable: true
            Layout.fillWidth: true
        }
        SAKCheckBox {
            id: appendCrcCheckbox
            text: qsTr("Append CRC(before suffix)");
            settingKey: settingKeys.inputCrcAppendEnable
            Layout.columnSpan: 2
        }
        SAKLabel {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("The start index is start from header, the first byte is 0. The end index is from tail, the last byte is 0. If parameters are set error, all bytes inputed will be calculated.")
        }
    }
}
