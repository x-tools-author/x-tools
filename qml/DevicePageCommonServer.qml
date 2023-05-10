import QtQuick
import QtQuick.Layouts

import "common"

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

    EDLabel {
        id: titleLabel
        Layout.columnSpan: 2
        font.weight: Font.Bold
    }
    EDLabel {
        text: qsTr("Address")
    }
    EDIpComboBox {
        id: addressComboBox
        editable: true
        settingKey: settingKeys.serverIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    EDLabel {
        text: qsTr("Port")
    }
    EDTextField {
        id: portTextField
        settingKey: settingKeys.serverPort
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    EDLabel {
        text: qsTr("Clients")
    }
    EDComboBox {
        id: clientsComboBox
        model: clients
        Layout.fillWidth: true
    }
    EDLabel {
        text: qsTr("Message")
        visible: showWebSocketInfo
    }
    EDComboBox {
        id: messageComboBox
        model: ["Bin", "Text"]
        visible: showWebSocketInfo
        settingKey: settingKeys.messageType
        Layout.fillWidth: true
    }
    Item { Layout.fillHeight: true }
}
