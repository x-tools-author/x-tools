import QtQuick
import QtQuick.Controls

import SAK.Custom

SAKComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        ListElement{text: qsTr("None"); value: SAKDataStructure.EscapeCharacterOptionNone}
        ListElement{text: "\\r"; value: SAKDataStructure.EscapeCharacterOptionR}
        ListElement{text: "\\n"; value: SAKDataStructure.EscapeCharacterOptionN}
        ListElement{text: "\\r\\n"; value: SAKDataStructure.EscapeCharacterOptionRN}
        ListElement{text: "\\n\\r"; value: SAKDataStructure.EscapeCharacterOptionNR}
        ListElement{text: "\\rAnd\\n"; value: SAKDataStructure.EscapeCharacterOptionRAndN}
    }
}
