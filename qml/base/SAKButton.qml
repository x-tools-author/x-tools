import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: root
    implicitHeight: 25
    implicitWidth: 70
    font.pixelSize: 12

    property alias color: label.color

    background: Rectangle {
        color: mouseArea.containsMouse ? "#FF01D766" : "#FF222222"
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true
            onClicked: {
               root.clicked()
            }
        }
    }

    contentItem: Text {
        id: label
        text: root.text
        font: root.font
        color: "#FFFFFF"
        smooth: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
