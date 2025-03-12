import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: UdpClient {}
    controllerSource: "qrc:/qml/ControllerUdpClient.qml"
    settingsGroup: "UdpClient"
}
