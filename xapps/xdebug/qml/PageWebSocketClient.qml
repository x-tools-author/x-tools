import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: WebSocketClient {}
    controllerSource: "qrc:/qml/ControllerWebSocketClient.qml"
    settingsGroup: "WebSocketClient"
}
