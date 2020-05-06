import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/component"

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "yellow"
        Text {
            anchors.centerIn: parent
            text: qsTr("终端输出")
        }
    }
}
