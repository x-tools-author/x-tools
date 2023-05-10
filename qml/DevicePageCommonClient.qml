import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "common"

GridLayout {
    id: root
    columns: 2

    property alias title: titleLabel.text
    property alias clientIp: clientIpAddressComboBox.editText
    property alias clientPort: clientPortTextField.text
    property alias specifyClientIpPort: specifyClientIpPortCheckBox.checked
    property alias serverIp: serverIpAddressComboBox.editText
    property alias serverPort: serverPortTextField.text
    property alias bindingIpPort: bindingInfoTextField.text
    property alias messageType: messageComboBox.currentIndex

    property bool showClientInfo: true
    property bool deviceIsWorking: false
    property bool showWebSocketInfo: false

    QtObject {
        id: settingKeys
        readonly property string clientIp: groupName + "/clientIp"
        readonly property string clientPort: groupName + "/clientPort"
        readonly property string specifyClientIpPort: groupName + "/specifyClientIpPort"
        readonly property string serverIp: groupName + "/serverIp"
        readonly property string serverPort: groupName + "/serverPort"
        readonly property string messageType: groupName + "/messageType"
    }

    EDLabel {
        id: titleLabel
        Layout.columnSpan: 2
        font.weight: Font.Bold
    }
    EDLabel {
        text: qsTr("Address")
        visible: showClientInfo
    }
    EDIpComboBox {
        id: clientIpAddressComboBox
        settingKey: settingKeys.clientIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
        visible: showClientInfo
    }
    EDLabel {
        text: qsTr("Port")
        visible: showClientInfo
    }
    EDTextField {
        id: clientPortTextField
        settingKey: settingKeys.clientPort
        enabled: !deviceIsWorking
        visible: showClientInfo
        Layout.fillWidth: true
    }
    EDLabel {
        text: qsTr("Binding")
        visible: showClientInfo
    }
    EDTextField {
        id: bindingInfoTextField
        enabled: false
        visible: showClientInfo
        Layout.fillWidth: true
    }
    EDCheckBox {
        id: specifyClientIpPortCheckBox
        text: qsTr("Specify client information")
        settingKey: settingKeys.specifyClientIpPort
        enabled: !deviceIsWorking
        visible: showClientInfo
        Layout.columnSpan: 2
    }
    EDLabel {
        text: qsTr("Server settings")
        Layout.columnSpan: 2
    }
    EDLabel {
        text: qsTr("Address")
    }
    EDIpComboBox {
        id: serverIpAddressComboBox
        settingKey: settingKeys.serverIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    EDLabel {
        text: qsTr("Port")
    }
    EDTextField {
        id: serverPortTextField
        settingKey: settingKeys.serverPort
        enabled: !deviceIsWorking
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
