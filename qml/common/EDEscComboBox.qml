import QtQuick
import QtQuick.Controls

import ED.EasyDebug

EDComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        ListElement{text: qsTr("None"); value: EDDataStructure.EscapeCharacterOptionNone}
        ListElement{text: "\\r"; value: EDDataStructure.EscapeCharacterOptionR}
        ListElement{text: "\\n"; value: EDDataStructure.EscapeCharacterOptionN}
        ListElement{text: "\\r\\n"; value: EDDataStructure.EscapeCharacterOptionRN}
        ListElement{text: "\\n\\r"; value: EDDataStructure.EscapeCharacterOptionNR}
        ListElement{text: "\\rAnd\\n"; value: EDDataStructure.EscapeCharacterOptionRAndN}
    }
}
