import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: TcpClient {}
    controllerSource: "qrc:/qml/ControllerTcpClient.qml"
    settingsGroup: "TcpClient"
}
