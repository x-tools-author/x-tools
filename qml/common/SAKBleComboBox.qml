import QtQuick
import QtQuick.Controls

import SAK.Custom

SAKComboBox {
    id: root
    model: ble.names

    property var bleController: ble

    SAKBleScanner {
        id: ble
    }
    BusyIndicator {
        anchors.centerIn: parent
        visible: running
    }
}
