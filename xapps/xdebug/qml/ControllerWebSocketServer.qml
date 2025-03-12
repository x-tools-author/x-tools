import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerSocket {
    id: root
    showClient: false
    showServer: true
    showWritTo: true
    showDataChannel: true
    showAuthentication: false
    settingsGroup: "WebSocketServer"

    Connections {
        target: device
        function onClientsChanged() {
            var clients = device.clients()
            setClientsModel(clients)
        }
    }
}
