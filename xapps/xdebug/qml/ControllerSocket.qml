import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerBase {
    id: root

    property Socket socket: device

    property bool showClient: true
    property bool showServer: true
    property bool showWritTo: true
    property bool showDataChannel: true
    property bool showAuthentication: true
    required property string settingsGroup
    readonly property string allClients: qsTr("All Clients")

    IpScanner {
        id: ipScanner
        enableIpV6: false
        enableAutoRefresh: !isWorking
    }

    QtObject {
        id: settingKeys
        readonly property string clientAddress: "clientAddress"
        readonly property string clientPort: "clientPort"
        readonly property string serverAddress: "serverAddress"
        readonly property string serverPort: "serverPort"
        readonly property string dataChannel: "dataChannel"
        readonly property string authentication: "authentication"
        readonly property string username: "username"
        readonly property string password: "password"

        readonly property string parameters: settingsGroup + "parameters"
    }

    Label {
        text: qsTr("Client address")
        visible: root.showClient
    }
    ControlsComboBox {
        id: clientAddressComboBox
        model: ipScanner.ipList
        visible: root.showClient
        enabled: !isWorking
        onActivated: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        visible: root.showClient
        text: qsTr("Client port")
    }
    ControlsSpinBox {
        id: clientPortSpinBox
        from: 1
        to: 65535
        value: 54321
        visible: root.showClient
        enabled: !isWorking
        onValueModified: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        visible: showServer
        text: qsTr("Server address")
    }
    ControlsComboBox {
        id: serverAddressComboBox
        model: ipScanner.ipList
        visible: showServer
        enabled: !isWorking
        editable: true
        onActivated: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        visible: showServer
        text: qsTr("Server port")
    }
    ControlsSpinBox {
        id: serverPortSpinBox
        from: 1
        to: 65535
        value: 12345
        visible: showServer
        enabled: !isWorking
        editable: true
        onValueModified: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        visible: showWritTo
        text: qsTr("Write to")
    }
    ControlsComboBox {
        id: writeToComboBox
        textRole: "text"
        valueRole: "value"
        model: ListModel {
            id: clientsModel
        }
        visible: showWritTo
        onActivated: {
            if (xApp.isSocketServer(communicator)) {
                var flag = writeToComboBox.valueAt(writeToComboBox.currentIndex)
                communicator.setCurrentClientFlag(flag)
            }
        }
        Layout.fillWidth: true
    }
    Label {
        visible: root.showDataChannel
        text: qsTr("Data channel")
    }
    ControlsComboBox {
        id: dataChannelComboBox
        textRole: "text"
        valueRole: "value"
        visible: root.showDataChannel
        onActivated: {
            writeSettings()

            var dataChannel = dataChannelComboBox.valueAt(dataChannelComboBox.currentIndex)
            console.info("Data channel: " + dataChannel)
            socket.setDataChannel(dataChannel)
        }
        Layout.fillWidth: true
        model: ListModel {
            ListElement {
                value: 0
                text: qsTr("Text")
            }
            ListElement {
                value: 1
                text: qsTr("Binary")
            }
        }
    }
    ControlsCheckBox {
        id: authenticationCheckBox
        visible: showAuthentication
        text: qsTr("Enable authentication")
        onClicked: writeSettings()
    }
    Label {
        visible: showAuthentication
        text: qsTr("Username")
    }
    ControlsTextField {
        id: usernameTextField
        visible: showAuthentication
        enabled: !isWorking
        onTextEdited: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        visible: showAuthentication
        text: qsTr("Password")
    }
    ControlsTextField {
        id: passwordTextField
        visible: showAuthentication
        enabled: !isWorking
        onTextEdited: writeSettings()
        Layout.fillWidth: true
    }
    Component.onCompleted: {
        readSettings()
        setClientsModel([])
    }

    function setClientsModel(model) {
        var displayText = writeToComboBox.displayText
        clientsModel.clear()

        var allClientsItem = {}
        allClientsItem["text"] = allClients
        allClientsItem["value"] = ""
        clientsModel.append(allClientsItem)
        for (var i = 0; i < model.length; i++) {
            var item = {}
            //var strList = model[i].split(":")
            item["text"] = model[i]
            item["value"] = model[i]
            clientsModel.append(item)
        }

        var index = writeToComboBox.find(displayText)
        writeToComboBox.currentIndex = index === -1 ? 0 : index
    }

    function readSettings() {
        var parameters = xSettings.value(settingKeys.parameters)
        if (parameters) {
            load(parameters)
        }
    }

    function writeSettings() {
        var parameters = save()
        var str = JSON.stringify(parameters)
        xSettings.setJsonObjectStringValue(settingKeys.parameters, str)
    }

    function load(parameters) {
        var clientAddress = parameters[settingKeys.clientAddress]
        var clientPort = parameters[settingKeys.clientPort]
        var serverAddress = parameters[settingKeys.serverAddress]
        var serverPort = parameters[settingKeys.serverPort]
        var dataChannel = parameters[settingKeys.dataChannel]
        var authentication = parameters[settingKeys.authentication]
        var username = parameters[settingKeys.username]
        var password = parameters[settingKeys.password]

        var index = clientAddressComboBox.find(clientAddress)
        clientAddressComboBox.currentIndex = index === -1 ? 0 : index
        index = serverAddressComboBox.find(serverAddress)
        serverAddressComboBox.currentIndex = index === -1 ? 0 : index
        index = dataChannelComboBox.find(dataChannel)
        dataChannelComboBox.currentIndex = index === -1 ? 0 : index

        clientPortSpinBox.value = clientPort
        serverPortSpinBox.value = serverPort
        authenticationCheckBox.checked = authentication === xTrue ? true : false
        usernameTextField.text = username
        passwordTextField.text = password
    }

    function save() {
        var parameters = {}
        parameters[settingKeys.clientAddress] = clientAddressComboBox.displayText
        parameters[settingKeys.clientPort] = Number(clientPortSpinBox.value)
        parameters[settingKeys.serverAddress] = serverAddressComboBox.displayText
        parameters[settingKeys.serverPort] = Number(serverPortSpinBox.value)
        parameters[settingKeys.dataChannel] = dataChannelComboBox.currentValue
        parameters[settingKeys.authentication] = authenticationCheckBox.checked
        parameters[settingKeys.username] = usernameTextField.text
        parameters[settingKeys.password] = passwordTextField.text
        return parameters
    }
}
