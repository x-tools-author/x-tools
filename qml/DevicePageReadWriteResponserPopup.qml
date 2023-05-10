import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "common"

EDPopup {
    id: root

    readonly property string itemEnable:        responserTool.itemEnable
    readonly property string itemDescription:   responserTool.itemDescription
    readonly property string itemOption:        responserTool.itemOption

    readonly property string itemReferenceEscapeCharacter:  responserTool.itemReferenceEscapeCharacter
    readonly property string itemReferenceTextFormat:       responserTool.itemReferenceTextFormat
    readonly property string itemReferencePrefix:           responserTool.itemReferencePrefix
    readonly property string itemReferenceSuffix:           responserTool.itemReferenceSuffix
    readonly property string itemReferenceInterval:         "itemReferenceInterval"
    readonly property string itemReferenceText:             responserTool.itemReferenceText
    readonly property string itemReferenceCrcEnable:        responserTool.itemReferenceCrcEnable
    readonly property string itemReferenceCrcAlgorithm:     responserTool.itemReferenceCrcAlgorithm
    readonly property string itemReferenceCrcStartIndex:    responserTool.itemReferenceCrcStartIndex
    readonly property string itemReferenceCrcEndIndex:      responserTool.itemReferenceCrcEndIndex

    readonly property string itemResponseEscapeCharacter:   responserTool.itemResponseEscapeCharacter
    readonly property string itemResponseTextFormat:        responserTool.itemResponseTextFormat
    readonly property string itemResponsePrefix:            responserTool.itemResponsePrefix
    readonly property string itemResponseSuffix:            responserTool.itemResponseSuffix
    readonly property string itemResponseInterval:          responserTool.itemResponseInterval
    readonly property string itemResponseText:              responserTool.itemResponseText
    readonly property string itemResponseCrcEnable:         responserTool.itemResponseCrcEnable
    readonly property string itemResponseCrcAlgorithm:      responserTool.itemResponseCrcAlgorithm
    readonly property string itemResponseCrcStartIndex:     responserTool.itemResponseCrcStartIndex
    readonly property string itemResponseCrcEndIndex:       responserTool.itemResponseCrcEndIndex

    readonly property string itemEscapeCharacter:   parameterIndex === 0 ? itemReferenceEscapeCharacter : itemResponseEscapeCharacter
    readonly property string itemTextFormat:        parameterIndex === 0 ? itemReferenceTextFormat      : itemResponseTextFormat
    readonly property string itemPrefix:            parameterIndex === 0 ? itemReferencePrefix          : itemResponsePrefix
    readonly property string itemSuffix:            parameterIndex === 0 ? itemReferenceSuffix          : itemResponseSuffix
    readonly property string itemInterval:          parameterIndex === 0 ? itemReferenceInterval        : itemResponseInterval
    readonly property string itemText:              parameterIndex === 0 ? itemReferenceText            : itemResponseText
    readonly property string itemCrcEnable:         parameterIndex === 0 ? itemReferenceCrcEnable       : itemResponseCrcEnable
    readonly property string itemCrcAlgorithm:      parameterIndex === 0 ? itemReferenceCrcAlgorithm    : itemResponseCrcAlgorithm
    readonly property string itemCrcStartIndex:     parameterIndex === 0 ? itemReferenceCrcStartIndex   : itemResponseCrcStartIndex
    readonly property string itemCrcEndIndex:       parameterIndex === 0 ? itemReferenceCrcEndIndex     : itemResponseCrcEndIndex

    property int parameterIndex: 0


    contentItem: ColumnLayout {
        GridLayout {
            Layout.fillWidth: true
            columns: 4
            columnSpacing: 8
            EDCheckBox {
                id: itemEnableCheckBox
                text: qsTr("Enable the item")
                Layout.fillWidth: true
                Layout.columnSpan: 4
            }
            EDLabel {
                text: qsTr("Description")
            }
            EDTextField {
                id: itemDescriptionTextField
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Response options")
            }
            EDResponseOptionsComboBox {
                id: itemOptionComboBox
                Layout.fillWidth: true
            }
            EDHorizontalLine {
                autoScale: false
                Layout.fillWidth: true
                Layout.columnSpan: 4
            }
            RowLayout {
                spacing: 8
                Layout.fillWidth: true
                Layout.columnSpan: 4
                Repeater {
                    id: itemRepeter
                    model: [qsTr("Reference"), qsTr("Response")]
                    GroupBox {
                        title: modelData
                        Layout.fillWidth: true
                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            EDLabel {
                                text: qsTr("Text format")
                                Layout.fillWidth: true
                            }
                            EDTextFormatComboBox {
                                id: itemTextFormatComboBox
                                Layout.fillWidth: true
                                onActivated: itemTextTextField.clear()
                            }
                            EDLabel {
                                text: qsTr("Item escape")
                                Layout.fillWidth: true
                            }
                            EDEscComboBox {
                                id: itemEscapeCharacterComboBox
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("Item prefix")
                                Layout.fillWidth: true
                            }
                            EDEscComboBox {
                                id: itemPrefixComboBox
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("Item suffix")
                                Layout.fillWidth: true
                            }
                            EDEscComboBox {
                                id: itemSuffixComboBox
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("Item interval")
                                Layout.fillWidth: true
                            }
                            EDSpinBox {
                                id: itemIntervalSpinBox
                                enabled: index === 1
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("Item text")
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                            }
                            EDTextField {
                                id: itemTextTextField
                                validator: itemTextFormatComboBox.currentValidator
                                Layout.fillWidth: true
                                Layout.minimumWidth: 300
                                Layout.columnSpan: 2
                            }
                            EDCheckBox {
                                id: itemCrcEnableCheckBox
                                text: qsTr("Append CRC data")
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                            }
                            EDLabel {
                                text: qsTr("CRC arithmetic")
                            }
                            EDCrcComboBox {
                                id: itemCrcArithmeticComboBox
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("CRC start index")
                            }
                            EDSpinBox {
                                id: itemCrcStartIndexSpinBox
                                Layout.fillWidth: true
                            }
                            EDLabel {
                                text: qsTr("CRC end index")
                            }
                            EDSpinBox {
                                id: itemCrcEndIndexSpinBox
                                Layout.fillWidth: true
                            }
                        }

                        function setParameters(parameters) {
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
                            itemIntervalSpinBox.value = parameters[itemInterval] ? parameters[itemInterval] : 1000
                            itemTextTextField.text = parameters[itemText]

                            itemCrcEnableCheckBox.checked = parameters[itemCrcEnable]
                            ret = itemCrcArithmeticComboBox.indexOfValue(parameters[itemCrcAlgorithm])
                            if (ret >= 0) {
                                itemCrcArithmeticComboBox.currentIndex = ret
                            }
                            itemCrcStartIndexSpinBox.value = parameters[itemCrcStartIndex]
                            itemCrcEndIndexSpinBox.value = parameters[itemCrcEndIndex]
                        }

                        function getParameters() {
                            var parameters = {}
                            parameters[itemEscapeCharacter] = itemEscapeCharacterComboBox.currentValue
                            parameters[itemTextFormat] = itemTextFormatComboBox.currentValue
                            parameters[itemPrefix] = itemPrefixComboBox.currentValue
                            parameters[itemSuffix] = itemSuffixComboBox.currentValue
                            parameters[itemInterval] = itemIntervalSpinBox.value
                            parameters[itemText] = itemTextTextField.text

                            parameters[itemCrcEnable] = itemCrcEnableCheckBox.checked
                            parameters[itemCrcAlgorithm] = itemCrcArithmeticComboBox.currentValue
                            parameters[itemCrcStartIndex] = itemCrcStartIndexSpinBox.value
                            parameters[itemCrcEndIndex] = itemCrcEndIndexSpinBox.value

                            return parameters
                        }
                    }

                    function setParameters(parameters, index) {
                        parameterIndex = index
                        var item = itemRepeter.itemAt(index)
                        if (item) {
                            item.setParameters(parameters)
                        }
                    }

                    function getParameters(index) {
                        parameterIndex = index
                        var item = itemRepeter.itemAt(index)
                        if (item) {
                            return item.getParameters()
                        }

                        return {}
                    }
                }
            }
        }
        Row {
            spacing: 8
            Layout.fillWidth: true
            Layout.columnSpan: 4
            Repeater {
                model: [qsTr("Cancle"), qsTr("Apply")]
                EDButton {
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
            itemEnableCheckBox.checked = parameters[itemEnable]
            itemDescriptionTextField.text = parameters[itemDescription]
            var ret = itemOptionComboBox.indexOfValue(parameters[itemOption])
            if (ret >= 0) {
                itemOptionComboBox.currentIndex = ret
            }

            itemRepeter.setParameters(parameters, 0)
            itemRepeter.setParameters(parameters, 1)
        }

        console.info("Set parameters:", JSON.stringify(parameters))
    }

    function getParameters() {
        var parameters = {}
        parameters[itemEnable] = itemEnableCheckBox.checked
        parameters[itemDescription] = itemDescriptionTextField.text
        parameters[itemOption] = itemOptionComboBox.currentValue

        var reference = itemRepeter.getParameters(0)
        var response = itemRepeter.getParameters(1)
        var referenceKeys = Object.keys(reference)
        var responseKeys = Object.keys(response)

        for (var i = 0; i < referenceKeys.length; i++) {
            parameters[referenceKeys[i]] = reference[referenceKeys[i]]
        }
        for (i = 0; i < responseKeys.length; i++) {
            parameters[responseKeys[i]] = response[responseKeys[i]]
        }

        console.info("Get parameters:", JSON.stringify(parameters))
        return parameters
    }
}
