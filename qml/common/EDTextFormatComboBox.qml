import QtQuick

import SAK.Custom

EDComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        id: listModel
        ListElement { text: "Bin"; value: EDDataStructure.TextFormatBin }
        ListElement { text: "Otc"; value: EDDataStructure.TextFormatOct }
        ListElement { text: "Dec"; value: EDDataStructure.TextFormatDec }
        ListElement { text: "Hex"; value: EDDataStructure.TextFormatHex }
        ListElement { text: "Ascii"; value: EDDataStructure.TextFormatAscii }
        ListElement { text: "Utf8"; value: EDDataStructure.TextFormatUtf8 }
    }

    property bool enableBin: true
    property bool enableOtc: true
    property bool enableDec: true
    property bool enableHex: true
    property bool enableAscii: true
    property bool enableUtf8: true

    EDBinRegularExpressionValidator{ id: binREV }
    EDOctRegularExpressionValidator{ id: octREV }
    EDDecRegularExpressionValidator{ id: decREV }
    EDHexRegularExpressionValidator{ id: hexREV }
    EDAsciiRegularExpressionValidator{ id: asciiREV }

    property var currentValidator:  {
        if (currentValue === EDDataStructure.TextFormatBin) {
            return binREV
        } else if (currentValue === EDDataStructure.TextFormatOct) {
            return octREV
        } else if (currentValue === EDDataStructure.TextFormatDec) {
            return decREV
        } else if (currentValue === EDDataStructure.TextFormatHex) {
            return hexREV
        } else if (currentValue === EDDataStructure.TextFormatAscii) {
            return asciiREV
        } else {
            return null
        }
    }

    Component.onCompleted: {
        if (!enableBin) {
            var ret = root.indexOfValue(EDDataStructure.TextFormatBin)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableOtc) {
            ret = root.indexOfValue(EDDataStructure.TextFormatOct)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableDec) {
            ret = root.indexOfValue(EDDataStructure.TextFormatDec)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableHex) {
            ret = root.indexOfValue(EDDataStructure.TextFormatHex)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableAscii) {
            ret = root.indexOfValue(EDDataStructure.TextFormatAscii)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }

        if (!enableUtf8) {
            ret = root.indexOfValue(EDDataStructure.TextFormatUtf8)
            if (ret !== -1) {
                listModel.remove(ret)
            }
        }
    }
}
