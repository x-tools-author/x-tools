import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

SAKPopup {
    id: root

    readonly property string itemDescription: emitterTool.itemDescription
    readonly property string itemEscapeCharacter: emitterTool.itemEscapeCharacter
    readonly property string itemTextFormat: emitterTool.itemTextFormat
    readonly property string itemPrefix: emitterTool.itemPrefix
    readonly property string itemSuffix: emitterTool.itemSuffix
    readonly property string itemText: emitterTool.itemText
    readonly property string itemCrcEnable: emitterTool.itemCrcEnable
    readonly property string itemCrcAlgorithm: emitterTool.itemCrcAlgorithm
    readonly property string itemCrcStartIndex: emitterTool.itemCrcStartIndex
    readonly property string itemCrcEndIndex: emitterTool.itemCrcEndIndex

    contentItem:  ColumnLayout {
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            SAKLabel {
                text: qsTr("Description")
            }
            SAKTextField {
                id: itemDescriptionTextField
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("Item text format")
                Layout.fillWidth: true
            }
            SAKTextFormatComboBox {
                id: itemTextFormatComboBox
                Layout.fillWidth: true
                onActivated: itemTextTextField.clear()
            }
            SAKLabel {
                text: qsTr("Item escape")
                Layout.fillWidth: true
            }
            SAKEscComboBox {
                id: itemEscapeCharacterComboBox
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("Item prefix")
                Layout.fillWidth: true
            }
            SAKEscComboBox {
                id: itemPrefixComboBox
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("Item suffix")
                Layout.fillWidth: true
            }
            SAKEscComboBox {
                id: itemSuffixComboBox
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("Item text")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            SAKTextField {
                id: itemTextTextField
                validator: itemTextFormatComboBox.currentValidator
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                Layout.columnSpan: 2
            }
            SAKCheckBox {
                id: itemCrcEnableCheckBox
                text: qsTr("Append CRC data")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            SAKLabel {
                text: qsTr("CRC arithmetic")
            }
            SAKCrcComboBox {
                id: itemCrcArithmeticComboBox
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("CRC start index")
            }
            SAKSpinBox {
                id: itemCrcStartIndexSpinBox
                Layout.fillWidth: true
            }
            SAKLabel {
                text: qsTr("CRC end index")
            }
            SAKSpinBox {
                id: itemCrcEndIndexSpinBox
                Layout.fillWidth: true
            }
        }
        Row {
            spacing: 8
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Repeater {
                model: [qsTr("Cancle"), qsTr("Apply")]
                SAKButton {
                    text: modelData
                    width: Math.round((parent.width - parent.spacing)/2)
                    onClicked: {
                        if (index === 0) {
                            root.rejected()
                        } else {
                            root.accepted()
                        }
                    }
                }
            }
        }
    }

    function setParameters(parameters) {
        if (parameters) {
            itemDescriptionTextField.text = parameters[itemDescription]
            var ret = itemTextFormatComboBox.indexOfValue(parameters[itemTextFormat])
            if (ret >= 0) {
                itemTextFormatComboBox.currentIndex = ret
            }
            ret = itemEscapeCharacterComboBox.indexOfValue(parameters[itemEscapeCharacter])
            if (ret >= 0) {
                itemEscapeCharacterComboBox.currentIndex = ret
            }
            ret = itemPrefixComboBox.indexOfValue(parameters[itemPrefix])
            if (ret >= 0) {
                itemPrefixComboBox.currentIndex = ret
            }
            ret = itemSuffixComboBox.indexOfValue(parameters[itemSuffix])
            if (ret >= 0) {
                itemSuffixComboBox.currentIndex = ret
            }
            itemTextTextField.text = parameters[itemText]

            itemCrcEnableCheckBox.checked = parameters[itemCrcEnable]
            ret = itemCrcArithmeticComboBox.indexOfValue(parameters[itemCrcAlgorithm])
            if (ret >= 0) {
                itemCrcArithmeticComboBox.currentIndex = ret
            }
            itemCrcStartIndexSpinBox.value = parameters[itemCrcStartIndex]
            itemCrcEndIndexSpinBox.value = parameters[itemCrcEndIndex]
        }
    }

    function getParameters() {
        var parameters = {}
        parameters[itemDescription] = itemDescriptionTextField.text
        parameters[itemEscapeCharacter] = itemEscapeCharacterComboBox.currentValue
        parameters[itemTextFormat] = itemTextFormatComboBox.currentValue
        parameters[itemPrefix] = itemPrefixComboBox.currentValue
        parameters[itemSuffix] = itemSuffixComboBox.currentValue
        parameters[itemTextFormat] = itemTextFormatComboBox.currentValue
        parameters[itemText] = itemTextTextField.text

        parameters[itemCrcEnable] = itemCrcEnableCheckBox.checked
        parameters[itemCrcAlgorithm] = itemCrcArithmeticComboBox.currentValue
        parameters[itemCrcStartIndex] = itemCrcStartIndexSpinBox.value
        parameters[itemCrcEndIndex] = itemCrcEndIndexSpinBox.value
        return parameters
    }
}
