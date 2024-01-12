import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "../common"
import "../toolbox"

SAKPane {
    id: root
    padding: 0

    property alias deviceController: clientController

    ToolBoxCommonClient {
        id: clientController
        anchors.fill: parent
        title: qsTr("UDP client settings")
    }
}
