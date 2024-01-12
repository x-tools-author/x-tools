import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "../common"
import "../toolbox"

SAKPane {
    id: root
    padding: 0

    property alias deviceController: controller

    ToolBoxCommonServer {
        id: controller
        anchors.fill: parent
        showWebSocketInfo: true
        title: qsTr("WebSocket server settings")
    }
}
