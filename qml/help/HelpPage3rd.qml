import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "red"
        Text {
            anchors.centerIn: parent
            text: qsTr("使用工具")
        }
    }
}
