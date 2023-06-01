import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

SAKPane {
    id: root
    padding: 0

    property alias deviceController: controller

    DevicePageCommonServer {
        id: controller
        anchors.fill: parent
        title: qsTr("UDP server settings")
    }
}
