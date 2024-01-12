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

    ToolBoxCommonClient {
        id: controller
        anchors.fill: parent
        title: qsTr("TCP client settings")
    }
}
