import QtQuick
import QtQuick.Controls

import SAK.Custom

SAKComboBox {
    id: root
    model: ble.names

    property var bleController: ble

    EDBle {
        id: ble
    }
    BusyIndicator {
        anchors.centerIn: parent
        running: ble.isDiscovering
        visible: running
    }
}
