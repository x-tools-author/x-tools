import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Qt.labs.qmlmodels

import xPing

Rectangle {
    id: root
    color: "#00000000"
    border.width: 0

    property alias text: label.text
    property bool enableTextColor: false
    property alias topBorderVisible: topBorder.visible
    property alias leftBorderVisible: leftBorder.visible
    property alias enableBackground: backgroundRectangle.visible
    property int rawStatus: XTableModel.RawCompareUnknown
    property bool isPinging: false
    property bool busyIndicatorVisible: false

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: xPingPrimaryColor
        opacity: 0.25
        visible: false
        border.width: 0
    }

    BusyIndicator {
        id: busyIndicator
        anchors.fill: parent
        visible: busyIndicatorVisible && isPinging
    }

    Label {
        id: label
        anchors.centerIn: parent
        color: {
            if (!enableTextColor) {
                return Material.foreground
            }

            if (rawStatus === XTableModel.RawStatusOnline) {
                return xPingGreenColor
            } else if (rawStatus === XTableModel.RawStatusOffline) {
                return xPingRedColor
            } else {
                return Material.foreground
            }
        }
    }

    Rectangle {
        height: 2
        width: root.width
        anchors.bottom: parent.bottom
        color: xPingPrimaryColor
    }
    Rectangle {
        width: 2
        height: root.height
        anchors.right: parent.right
        color: xPingPrimaryColor
    }
    Rectangle {
        id: topBorder
        height: 2
        width: root.width
        anchors.top: parent.top
        visible: false
        color: xPingPrimaryColor
    }
    Rectangle {
        id: leftBorder
        width: 2
        height: root.height
        visible: false
        color: xPingPrimaryColor
    }
}
