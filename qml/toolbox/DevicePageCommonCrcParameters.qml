import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../common"

SAKGroupBox {
    title: qsTr("CRC Parameters")
    Layout.fillWidth: true

    property alias parameterKeys: parameterKeysObject

    QtObject {
        id: parameterKeysObject

        readonly property string crcEnable: "crcEnable"
        readonly property string crcArithmetic: "crcArithmetic"
        readonly property string crcStartIndex: "crcStartIndex"
        readonly property string crcEndIndex: "crcEndIndex"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        SAKCheckBox {
            id: crcEnableCheckBox
            text: qsTr("CRC enable")
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
        SAKLabel {
            text: qsTr("CRC arithmetic")
        }
        SAKCrcComboBox {
            id: crcArithmeticComboBox
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("CRC start index")
        }
        SAKSpinBox {
            id: crcStartIndexSpinBox
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("CRC end index")
        }
        SAKSpinBox {
            id: crcEndIndexSpinBox
            Layout.fillWidth: true
        }
    }

    function getParameters() {
        var parameters = {}
        parameters[parameterKeys.crcEnable] = crcEnableCheckBox.checked
        parameters[parameterKeys.crcArithmetic] = crcArithmeticComboBox.currentValue
        parameters[parameterKeys.crcStartIndex] = crcStartIndexSpinBox.value
        parameters[parameterKeys.crcEndIndex] = crcEndIndexSpinBox.value

        return parameters
    }

    function setParameters(parameters) {
        crcEnableCheckBox.checked = parameters[parameterKeys.crcEnable]
        var ret = crcArithmeticComboBox.indexOfValue(parameters[parameterKeys.crcArithmetic])
        if (ret >= 0) {
            crcArithmeticComboBox.currentIndex = ret
        }
        crcStartIndexSpinBox.value = parameters[parameterKeys.crcStartIndex]
        crcEndIndexSpinBox.value = parameters[parameterKeys.crcEndIndex]
    }

    function resetParameters() {
        crcEnableCheckBox.checked = true
        crcArithmeticComboBox.currentIndex = 0
        crcStartIndexSpinBox.value = 0
        crcEndIndexSpinBox.value = 0
    }
}
