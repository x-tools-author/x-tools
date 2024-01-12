import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "../common"

SAKPane {
    id: root
    padding: 0

    property var portNames: []
    property var baudRates: []

    property SAKSerialport comTool: null
    property bool devIsWorking: comTool ? comTool.isWorking : false
    property bool ignoredUpdate: comTool ? comTool.isWorking : false

    signal invokeRefreshDevice

    onIgnoredUpdateChanged: spController.setIgnoredUpdate(ignoredUpdate)

    QtObject {
        id: settingKeys

        readonly property string portName: groupName + "/portNmae"
        readonly property string baudRate: groupName + "/baudRate"
        readonly property string dataBits: groupName + "/dataBits"
        readonly property string stopBits: groupName + "/stopBits"
        readonly property string parity: groupName + "/parity"
        readonly property string flowControl: groupName + "/flowControl"
        readonly property string customBaudRate: groupName + "/customBaudRate"
        readonly property string ignoredBusyDevice: groupName + "/ignoredBusyDevice"
        readonly property string autoUpdatePortNames: groupName + "/autoUpdatePortNames"
    }

    SAKSerialPortScanner {
        id: spController
    }

    ListModel {
        id: additionListModel
        ListElement {
            text: qsTr("None")
            value: ""
        }
        ListElement {
            text: qsTr("\\r")
            value: "\r"
        }
        ListElement {
            text: qsTr("\\n")
            value: "\n"
        }
        ListElement {
            text: qsTr("\\r\\n")
            value: "\r\n"
        }
        ListElement {
            text: qsTr("\\n\\r")
            value: "\n\r"
        }
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        SAKLabel {
            Layout.columnSpan: 2
            text: qsTr("Serial port settings")
            font.weight: Font.Bold
            color: Material.foreground
        }
        SAKLabel {
            text: qsTr("Port name")
        }
        SAKComboBox {
            id: portNameComboBox
            model: spController.portNames
            enabled: !devIsWorking
            Layout.fillWidth: true
            onActivated: {
                sakSettings.setValue(settingKeys.portName, portNameComboBox.currentText)
            }
            Component.onCompleted: {
                var portName = sakSettings.value(settingKeys.portName)
                var ret = portNameComboBox.find(portName)
                if (ret !== -1) {
                    portNameComboBox.currentIndex = ret
                }
            }
        }
        SAKLabel {
            text: qsTr("Baud rate")
        }
        SAKComboBox {
            id: baudRateComboBox
            model: spController.baudRates
            editable: true
            enabled: !devIsWorking
            Layout.fillWidth: true
            onActivated: {
                writeBaudRateToIniFile()
            }
            Component.onCompleted: {
                var baudRate = sakSettings.value(settingKeys.baudRate)
                var defaultBaudRate = "9600"
                if (baudRate !== 0) {
                    defaultBaudRate = String(baudRate)
                }

                var ret = baudRateComboBox.find(defaultBaudRate)
                if (ret !== -1) {
                    baudRateComboBox.currentIndex = ret
                }
            }

            function writeBaudRateToIniFile() {
                var bd = Number(baudRateComboBox.currentText)
                sakSettings.setValue(settingKeys.baudRate, bd)
            }
        }
        Label {
            text: qsTr("Stop bits")
        }
        SAKComboBox {
            id: stopBitsComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement {
                    text: "1"
                    value: 1
                }
                ListElement {
                    text: "1.5"
                    value: 3
                }
                ListElement {
                    text: "2"
                    value: 2
                }
            }
            onActivated: {
                sakSettings.setValue(settingKeys.stopBits, stopBitsComboBox.currentValue)
            }
            Component.onCompleted: {
                var stopBits = sakSettings.value(settingKeys.stopBits)
                if (stopBits !== 0) {
                    var ret = stopBitsComboBox.indexOfValue(stopBits)
                    if (ret !== -1) {
                        stopBitsComboBox.currentIndex = ret
                    }
                }
            }
        }
        SAKLabel {
            text: qsTr("Data bits")
        }
        SAKComboBox {
            id: dataBitsComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement {
                    text: "8"
                    value: 8
                }
                ListElement {
                    text: "7"
                    value: 7
                }
                ListElement {
                    text: "6"
                    value: 6
                }
                ListElement {
                    text: "5"
                    value: 5
                }
            }
            onActivated: {
                sakSettings.setValue(settingKeys.dataBits, dataBitsComboBox.currentValue)
            }
            Component.onCompleted: {
                var dataBit = sakSettings.value(settingKeys.dataBits)
                if (dataBit !== 0) {
                    var ret = dataBitsComboBox.indexOfValue(dataBit)
                    if (ret !== -1) {
                        dataBitsComboBox.currentIndex = ret
                    }
                }
            }
        }
        SAKLabel {
            text: qsTr("Parity")
        }
        SAKComboBox {
            id: parityComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement {
                    text: qsTr("No")
                    value: 0
                }
                ListElement {
                    text: qsTr("Even")
                    value: 2
                }
                ListElement {
                    text: qsTr("Odd")
                    value: 3
                }
                ListElement {
                    text: qsTr("Space")
                    value: 4
                }
                ListElement {
                    text: qsTr("Mark")
                    value: 5
                }
            }
            onActivated: {
                sakSettings.setValue(settingKeys.parity, parityComboBox.currentValue)
            }
            Component.onCompleted: {
                var parity = sakSettings.value(settingKeys.parity)
                if (parity !== 0) {
                    var ret = parityComboBox.indexOfValue(parity)
                    if (ret !== -1) {
                        parityComboBox.currentIndex = ret
                    }
                }
            }
        }
        SAKLabel {
            text: qsTr("Flow control")
        }
        SAKComboBox {
            id: flowControlComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement {
                    text: qsTr("No")
                    value: 0
                }
                ListElement {
                    text: qsTr("Hardware")
                    value: 1
                }
                ListElement {
                    text: qsTr("Software")
                    value: 2
                }
            }
            onActivated: {
                sakSettings.setValue(settingKeys.flowControl, flowControlComboBox.currentValue)
            }
            Component.onCompleted: {
                var flowControl = sakSettings.value(settingKeys.flowControl)
                if (flowControl !== 0) {
                    var ret = flowControlComboBox.indexOfValue(flowControl)
                    if (ret !== -1) {
                        flowControlComboBox.currentIndex = ret
                    }
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

    function portName() {
        return portNameComboBox.currentText
    }

    function baudRate() {
        return parseInt(baudRateComboBox.currentText)
    }

    function dataBits() {
        return dataBitsComboBox.currentValue
    }

    function stopBits() {
        return stopBitsComboBox.currentValue
    }

    function parity() {
        return parityComboBox.currentValue
    }

    function flowControl() {
        return flowControlComboBox.currentValue
    }
}
