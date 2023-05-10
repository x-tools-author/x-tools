import QtQuick
import QtQuick.Controls

import ED.EasyDebug

EDComboBox {
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
