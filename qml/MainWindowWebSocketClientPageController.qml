import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

SAKPane {
    id: root
    padding: 0

    property alias deviceController: controller

    DevicePageCommonClient {
        id: controller
        anchors.fill: parent
        showClientInfo: false
        showWebSocketInfo: true
        title: qsTr("WebSocket client settings")
    }
}
