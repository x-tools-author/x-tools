import QtQuick
import QtQuick.Controls

import SAK.Custom

SAKComboBox {
    id: root
    valueRole: "value"
    textRole: "text"
    model: ListModel {
        //ListElement { text: qsTr("Disable"); value: EDDataStructure.ResponseOptionDisable }
        ListElement { text: qsTr("Echo"); value: EDDataStructure.ResponseOptionEcho }
        ListElement { text: qsTr("Aways"); value: EDDataStructure.ResponseOptionAways }
        ListElement { text: qsTr("InputEqualReference"); value: EDDataStructure.ResponseOptionInputEqualReference }
        ListElement { text: qsTr("InputContainReference"); value: EDDataStructure.ResponseOptionInputContainReference }
        ListElement { text: qsTr("InputDiscontainReference"); value: EDDataStructure.ResponseOptionInputDiscontainReference }
    }
}
