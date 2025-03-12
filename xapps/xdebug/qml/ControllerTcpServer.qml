import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerSocket {
    id: root
    showClient: false
    showServer: true
    showWritTo: true
    showDataChannel: false
    showAuthentication: false
    settingsGroup: "TcpServer"

    Connections {
        target: device
        function onClientsChanged() {
            var clients = communicator.clients()
            setClientsModel(clients)
        }
    }
}
