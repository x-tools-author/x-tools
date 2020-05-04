import QtQuick 2.12

import "qrc:/qml/base"

Rectangle {
    id: root
    height: 35
    color: "#80101010"

    property alias icon: icon.source
    property alias title: title.text

    Row {
        spacing: 10
        leftPadding: 5
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id: icon
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
        }

        SAKLabel {
            id: title
            color: "#999999"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

}
