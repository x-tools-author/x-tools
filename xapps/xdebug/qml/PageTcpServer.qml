import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: TcpServer {}
    controllerSource: "qrc:/qml/ControllerTcpServer.qml"
    settingsGroup: "TcpServer"
}
