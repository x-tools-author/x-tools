import QtQuick

import SAK.Custom

SAKComboBox {
    id: root
    editable: true
    model: networkInterface.ipList

    property alias enableIpV4: networkInterface.enableIpV4
    property alias enableIpV6: networkInterface.enableIpV6
    property alias enableAutoRefresh: networkInterface.enableAutoRefresh

    EDNetworkInterface {
        id: networkInterface
        enableAutoRefresh: root.enabled
    }
}
