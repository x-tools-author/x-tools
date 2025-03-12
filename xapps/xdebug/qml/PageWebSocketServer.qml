import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: WebSocketServer {}
    controllerSource: "qrc:/qml/ControllerWebSocketServer.qml"
    settingsGroup: "WebSocketServer"
}
