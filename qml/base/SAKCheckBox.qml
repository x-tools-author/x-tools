import QtQuick 2.12
import QtQuick.Controls 2.12

CheckBox {
    id: root
    font.pixelSize: 12
    indicator: Rectangle {
        width: 15
        height: width
        color: "#00000000"
        anchors.verticalCenter: root.verticalCenter
        border {
            width: 1
            color: "#535353"
        }

        Rectangle {
            width: parent.width*0.5
            height: width
            anchors.centerIn: parent
            color: "#01D766"
            visible: root.checked
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        color: "#FFFFFF"
        anchors.verticalCenter: root.verticalCenter
        anchors.left: parent.left
        leftPadding: 23
        verticalAlignment: Text.AlignVCenter
    }
}
