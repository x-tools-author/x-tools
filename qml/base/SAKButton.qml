import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: root
    implicitHeight: 25
    implicitWidth: 70
    font.pixelSize: 12

    property alias color: label.color
    property alias customEnable: mouseArea.enabled

    background: Rectangle {
        color: mouseArea.containsMouse ? "#FF01D766" : "#FF222222"
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: enabled ? Qt.PointingHandCursor : Qt.ForbiddenCursor
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
