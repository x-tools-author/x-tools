import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerSocket {
    id: root
    showClient: false
    showServer: true
    showWritTo: false
    showDataChannel: true
    showAuthentication: true
    settingsGroup: "WebSocketClient"
}
