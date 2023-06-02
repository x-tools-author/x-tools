import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../common"

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

    SAKLabel {
        id: titleLabel
        Layout.columnSpan: 2
        font.weight: Font.Bold
    }
    SAKLabel {
        text: qsTr("Address")
        visible: showClientInfo
    }
    SAKIpComboBox {
        id: clientIpAddressComboBox
        settingKey: settingKeys.clientIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
        visible: showClientInfo
    }
    SAKLabel {
        text: qsTr("Port")
        visible: showClientInfo
    }
    SAKTextField {
        id: clientPortTextField
        settingKey: settingKeys.clientPort
        enabled: !deviceIsWorking
        visible: showClientInfo
        Layout.fillWidth: true
    }
    SAKLabel {
        text: qsTr("Binding")
        visible: showClientInfo
    }
    SAKTextField {
        id: bindingInfoTextField
        enabled: false
        visible: showClientInfo
        Layout.fillWidth: true
    }
    SAKCheckBox {
        id: specifyClientIpPortCheckBox
        text: qsTr("Specify client information")
        settingKey: settingKeys.specifyClientIpPort
        enabled: !deviceIsWorking
        visible: showClientInfo
        Layout.columnSpan: 2
    }
    SAKLabel {
        text: qsTr("Server settings")
        Layout.columnSpan: 2
    }
    SAKLabel {
        text: qsTr("Address")
    }
    SAKIpComboBox {
        id: serverIpAddressComboBox
        settingKey: settingKeys.serverIp
        enabled: !deviceIsWorking
        Layout.fillWidth: true
    }
    SAKLabel {
        text: qsTr("Port")
    }
    SAKTextField {
        id: serverPortTextField
        settingKey: settingKeys.serverPort
        enabled: !deviceIsWorking
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
