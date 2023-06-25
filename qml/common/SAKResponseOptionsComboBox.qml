import QtQuick
import QtQuick.Controls

import SAK.Custom

SAKComboBox {
    id: root
    valueRole: "value"
    textRole: "text"
    model: ListModel {
        //ListElement { text: qsTr("Disable"); value: SAKDataStructure.ResponseOptionDisable }
        ListElement { text: qsTr("Echo"); value: SAKDataStructure.ResponseOptionEcho }
        ListElement { text: qsTr("Aways"); value: SAKDataStructure.ResponseOptionAlways }
        ListElement { text: qsTr("InputEqualReference"); value: SAKDataStructure.ResponseOptionInputEqualReference }
        ListElement { text: qsTr("InputContainReference"); value: SAKDataStructure.ResponseOptionInputContainReference }
        ListElement { text: qsTr("InputDiscontainReference"); value: SAKDataStructure.ResponseOptionInputDiscontainReference }
    }
}
