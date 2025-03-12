import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerSocket {
    id: root
    showClient: false
    showServer: true
    showWritTo: false
    showDataChannel: false
    showAuthentication: false
    settingsGroup: "UdpClient"
}
