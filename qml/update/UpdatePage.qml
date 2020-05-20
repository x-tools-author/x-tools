import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "green"
        Text {
            anchors.centerIn: parent
            text: qsTr("软件设置")
        }
    }
}
