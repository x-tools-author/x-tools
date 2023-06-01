import QtQuick
import QtQuick.Controls

SAKComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        ListElement{text: qsTr("None"); value: ""}
        ListElement{text: "\\r"; value: "\r"}
        ListElement{text: "\\n"; value: "\n"}
        ListElement{text: "\\r\\n"; value: "\r\n"}
        ListElement{text: "\\n\\r"; value: "\n\r"}
    }
}
