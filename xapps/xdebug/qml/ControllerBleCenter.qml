import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ControllerBase {
    id: root

    property var infos: []
    property var characteristics: []

    property bool writeFlag: false
    property bool writeNoResponseFlag: false
    property bool readFlag: false
    property bool notifyFlag: false
    property bool isNotify: false

    Timer {
        // 定时检查是否notify
        interval: 500
        //running: communicator && communicator.isWorking
        repeat: true
        onTriggered: {
            var service = serviceComboBox.currentValue
            var characteristic = characteristicsComboBox.currentValue
            isNotify = communicator.isNotify(service, characteristic)
        }
    }
    Connections {
        target: device
        function onServiceDiscovered(service) {
            var item = {}
            item['text'] = communicator.getServiceName(service)
            item['value'] = service
            servicesMode.append(item)
            if (servicesMode.count === 1) {
                serviceComboBox.currentIndex = 0
            }
        }
    }
    ButtonGroup {
        id: writeButtonsGroup
    }
    BleScanner {
        id: bleScanner
        // onDeviceDiscoveredForQml: function (name, info) {
        //     var item = {}
        //     item['value'] = info // 添加到model后通过currentValue获取的值为undefined,原因不详，用infos存储
        //     item['text'] = name
        //     infos.push(info)
        //     bleInfoModel.append(item)
        //     if (bleInfoModel.count === 1) {
        //         devicesComboBox.currentIndex = 0
        //     }
        // }
        // onErrorOccurred: function (errString) {
        //     console.info("Error", errString)
        //     mainWindow.showMessage(qsTr("Error"), errString)
        // }
    }
    QtObject {
        id: settingKeys
        readonly property string deviceInfo: "deviceInfo"
    }
    ControlsLabel {
        id: deviceListLabel
        text: qsTr("Device list")
    }
    ControlsComboBox {
        id: devicesComboBox
        valueRole: "value"
        textRole: "text"
        //enabled: communicator ? !communicator.isWorking : false
        Layout.fillWidth: true
        model: ListModel {
            id: bleInfoModel
        }
    }
    ProgressBar {
        id: progressBar
        indeterminate: true
        //visible: bleScanner.isBusy
        Layout.fillWidth: true
    }
    ControlsButton {
        text: bleScanner.isBusy ? qsTr("Stop") : qsTr("Scan")
        Layout.fillWidth: true
        //enabled: !communicator.isWorking
        onClicked: {
            if (bleScanner.isBusy) {
                bleScanner.stop()
            } else {
                infos = []
                bleInfoModel.clear()
                bleScanner.start()
            }
        }
    }
    ControlsLabel {
        text: qsTr("Services")
    }
    ControlsComboBox {
        id: serviceComboBox
        valueRole: "value"
        textRole: "text"
        enabled: servicesMode.count > 0
        onCurrentIndexChanged: updateCharacteristicList()
        Layout.fillWidth: true
        model: ListModel {
            id: servicesMode
        }
    }
    ControlsLabel {
        text: qsTr("Characteristics")
    }
    ControlsComboBox {
        id: characteristicsComboBox
        valueRole: "value"
        textRole: "text"
        enabled: servicesMode.count > 0
        onCurrentIndexChanged: updateControlStatus()
        Layout.fillWidth: true
        model: ListModel {
            id: characteristicsModel
        }
    }
    Label {
        visible: characteristicsComboBox.displayText === "Invalid"
        text: qsTr("Invalid characteristic")
        color: "red"
        Layout.fillWidth: true
    }
    RadioButton {
        id: writeWithResponseRadioButton
        text: qsTr("Write with response")
        visible: writeFlag
        onClicked: updateParameters()
        Component.onCompleted: writeButtonsGroup.addButton(writeWithResponseRadioButton)
    }
    RadioButton {
        id: writeWithoutResponseRadioButton
        text: qsTr("Write without response")
        visible: writeNoResponseFlag
        onClicked: updateParameters()
        Component.onCompleted: writeButtonsGroup.addButton(writeWithoutResponseRadioButton)
    }
    RowLayout {
        Layout.fillWidth: true
        visible: readFlag || notifyFlag
        ControlsButton {
            text: isNotify ? qsTr("Unnotify") : qsTr("Notify")
            enabled: readFlag
            Layout.fillWidth: true
            onClicked: {
                var service = serviceComboBox.currentValue
                var characteristic = characteristicsComboBox.currentValue
                communicator.switchNotify(service, characteristic)
            }
        }
        ControlsButton {
            text: qsTr("Read")
            enabled: readFlag
            Layout.fillWidth: true
            onClicked: {
                var service = serviceComboBox.currentValue
                var characteristic = characteristicsComboBox.currentValue
                communicator.readCharacteristic(service, characteristic)
            }
        }
    }

    function updateParameters() {
        var parameters = save()
        communicator.setParameters(parameters)
    }

    function updateControlStatus() {
        var index = characteristicsComboBox.currentIndex
        var characteristic = root.characteristics[index]
        readFlag = communicator.testReadFlag(characteristic)
        notifyFlag = communicator.testNotifyFlag(characteristic)
        writeFlag = communicator.testWriteFlag(characteristic)
        writeNoResponseFlag = communicator.testWriteNoResponseFlag(characteristic)

        if (notifyFlag) {
            communicator.readNotify(serviceComboBox.currentValue, characteristic)
        }

        if (writeFlag && writeNoResponseFlag) {
            if (!(writeWithResponseRadioButton.checked
                  && writeWithoutResponseRadioButton.checked)) {
                writeWithResponseRadioButton.checked = true
            }
        } else if (writeFlag) {
            writeWithResponseRadioButton.checked = true
        } else if (writeNoResponseFlag) {
            writeWithoutResponseRadioButton.checked = true
        }
    }

    function updateCharacteristicList() {
        characteristicsModel.clear()
        root.characteristics = []

        var service = serviceComboBox.currentValue
        var characteristics = communicator.getCharacteristics(service)
        for (var i = 0; i < characteristics.length; i++) {
            var characteristic = characteristics[i]
            var text = communicator.getCharacteristicName(characteristic)
            var value = characteristic

            var item = {}
            item['text'] = text
            item['value'] = characteristic
            root.characteristics.push(characteristic)
            characteristicsModel.append(item)
            if (characteristicsModel.count === 1) {
                characteristicsComboBox.currentIndex = 0
            }
        }
    }

    function aboutToOpen() {
        bleScanner.stop()
        servicesMode.clear()
        characteristicsModel.clear()
        root.characteristics = []
    }

    function save() {
        var parameters = {}
        var currentIndex = devicesComboBox.currentIndex
        parameters[settingKeys.deviceInfo] = infos[currentIndex]
        return parameters
    }
}
