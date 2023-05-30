import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

EDPane {
    id: root
    padding: 0

    property alias deviceController: clientController

    DevicePageCommonClient {
        id: clientController
        anchors.fill: parent
        title: qsTr("UDP client settings")
    }
}
