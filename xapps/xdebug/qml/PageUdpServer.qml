import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: UdpServer {}
    controllerSource: "qrc:/qml/ControllerUdpServer.qml"
    settingsGroup: "UdpServer"
}
