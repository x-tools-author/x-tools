import QtQuick

import SAK.Custom

SAKComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        id: listModel
        ListElement { text: "Bin"; value: SAKDataStructure.TextFormatBin }
        ListElement { text: "Otc"; value: SAKDataStructure.TextFormatOct }
        ListElement { text: "Dec"; value: SAKDataStructure.TextFormatDec }
        ListElement { text: "Hex"; value: SAKDataStructure.TextFormatHex }
        ListElement { text: "Ascii"; value: SAKDataStructure.TextFormatAscii }
        ListElement { text: "Utf8"; value: SAKDataStructure.TextFormatUtf8 }
    }

    property bool enableBin: true
    property bool enableOtc: true
    property bool enableDec: true
    property bool enableHex: true
    property bool enableAscii: true
    property bool enableUtf8: true

    SAKBinRegularExpressionValidator{ id: binREV }
    SAKOctRegularExpressionValidator{ id: octREV }
    SAKDecRegularExpressionValidator{ id: decREV }
    SAKHexRegularExpressionValidator{ id: hexREV }
    SAKAsciiRegularExpressionValidator{ id: asciiREV }

    property var currentValidator:  {
        if (currentValue === SAKDataStructure.TextFormatBin) {
            return binREV
        } else if (currentValue === SAKDataStructure.TextFormatOct) {
            return octREV
        } else if (currentValue === SAKDataStructure.TextFormatDec) {
            return decREV
        } else if (currentValue === SAKDataStructure.TextFormatHex) {
            return hexREV
        } else if (currentValue === SAKDataStructure.TextFormatAscii) {
            return asciiREV
        } else {
            return null
        }
    }

    Component.onCompleted: {
        if (!enableBin) {
            var ret = root.indexOfValue(SAKDataStructure.TextFormatBin)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableOtc) {
            ret = root.indexOfValue(SAKDataStructure.TextFormatOct)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableDec) {
            ret = root.indexOfValue(SAKDataStructure.TextFormatDec)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableHex) {
            ret = root.indexOfValue(SAKDataStructure.TextFormatHex)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableAscii) {
            ret = root.indexOfValue(SAKDataStructure.TextFormatAscii)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableUtf8) {
            ret = root.indexOfValue(SAKDataStructure.TextFormatUtf8)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }
    }
}
