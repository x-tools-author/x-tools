import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

EDPane {
    id: root
    padding: 0

    property alias deviceController: controller

    DevicePageCommonServer {
        id: controller
        anchors.fill: parent
        showWebSocketInfo: true
        title: qsTr("WebSocket server settings")
    }
}
