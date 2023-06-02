import QtQuick
import QtQuick.Layouts

import "../common"

GridLayout {
    id: root
    columns: 2

    property alias title: titleLabel.text
    property var clients: []
    property bool deviceIsWorking: false
    property alias serverIp: addressComboBox.editText
    property alias serverPort: portTextField.text
    property alias clientIndex: clientsComboBox.currentIndex
    property alias messageType: messageComboBox.currentIndex

    property bool showWebSocketInfo: false

    QtObject {
        id: settingKeys
        readonly property string serverIp: "serverIp"
        readonly property string serverPort: "serverPort"
        readonly property string messageType: "messageType"
    }

    SAKLabel {
        id: titleLabel
        Layout.columnSpan: 2
        font.weight: Font.Bold
    }
    SAKLabel {
        text: qsTr("Address")
    }
    SAKIpComboBox {
        id: addressComboBox
        editable: true
        settingKey: settingKeys.serverIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    SAKLabel {
        text: qsTr("Port")
    }
    SAKTextField {
        id: portTextField
        settingKey: settingKeys.serverPort
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    SAKLabel {
        text: qsTr("Clients")
    }
    SAKComboBox {
        id: clientsComboBox
        model: clients
        Layout.fillWidth: true
    }
    SAKLabel {
        text: qsTr("Message")
        visible: showWebSocketInfo
    }
    SAKComboBox {
        id: messageComboBox
        model: ["Bin", "Text"]
        visible: showWebSocketInfo
        settingKey: settingKeys.messageType
        Layout.fillWidth: true
    }
    Item { Layout.fillHeight: true }
}
