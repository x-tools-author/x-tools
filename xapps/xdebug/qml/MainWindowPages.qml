import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import xTools.xDebug

Item {
    id: root

    property alias currentPageIndex: pageStackLayout.currentIndex

    StackLayout {
        id: pageStackLayout
        anchors.fill: parent

        PageSerialPort {}
        PageBleCenter {}
        PageUdpClient {}
        PageUdpServer {}
        PageTcpClient {}
        PageTcpServer {}
        PageWebSocketClient {}
        PageWebSocketServer {}

        PageAbout {}
        PageSettings {}
    }
}
