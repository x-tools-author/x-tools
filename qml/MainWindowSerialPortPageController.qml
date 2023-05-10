import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ED.EasyDebug
import "common"

EDPane {
    id: root
    padding: 0

    property var portNames: []
    property var baudRates: []

    property EDSerialportTool comTool: null
    property bool devIsWorking: comTool ? comTool.isWorking : false
    property bool ignoredUpdate: comTool ? comTool.isWorking : false

    signal invokeRefreshDevice()

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

    EDSerialPort {
        id: spController
    }

    ListModel {
        id: additionListModel
        ListElement {text: qsTr("None"); value: ""}
        ListElement {text: qsTr("\\r"); value: "\r"}
        ListElement {text: qsTr("\\n"); value: "\n"}
        ListElement {text: qsTr("\\r\\n"); value: "\r\n"}
        ListElement {text: qsTr("\\n\\r"); value: "\n\r"}
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        EDLabel {
            Layout.columnSpan: 2
            text: qsTr("Serial port settings")
            font.weight: Font.Bold
            color: Material.foreground
        }
        EDLabel {
            text: qsTr("Port names")
        }
        EDComboBox {
            id: portNameComboBox
            model: spController.portNames
            enabled: !devIsWorking
            Layout.fillWidth: true
            onActivated: {
                edSettings.edSetValue(settingKeys.portName,
                                      portNameComboBox.currentText)
            }
            Component.onCompleted: {
                var portName = edSettings.edValue(settingKeys.portName)
                var ret = portNameComboBox.find(portName)
                if (ret !== -1) {
                    portNameComboBox.currentIndex = ret
                }
            }
        }
        EDLabel {
            text: qsTr("Baud rate")
        }
        EDComboBox {
            id: baudRateComboBox
            model: spController.baudRates
            editable: bdCheckBox.checked
            enabled: !devIsWorking
            Layout.fillWidth: true
            onActivated: {
                writeBaudRateToIniFile()
            }
            onCurrentTextChanged: {
                if (bdCheckBox.checked) {
                    writeBaudRateToIniFile()
                }
            }
            Component.onCompleted: {
                var baudRate = edSettings.edValue(settingKeys.baudRate)
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
                edSettings.edSetValue(settingKeys.baudRate, bd)
            }
        }
        Label {
            text: qsTr("Stop bits")
        }
        EDComboBox {
            id: stopBitsComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement{text: "1"; value: 1}
                ListElement{text: "1.5"; value: 3}
                ListElement{text: "2"; value: 2}
            }
            onActivated: {
                edSettings.edSetValue(settingKeys.stopBits, stopBitsComboBox.currentValue)
            }
            Component.onCompleted: {
                var stopBits = edSettings.edValue(settingKeys.stopBits)
                if (stopBits !== 0) {
                    var ret = stopBitsComboBox.indexOfValue(stopBits)
                    if (ret !== -1) {
                        stopBitsComboBox.currentIndex = ret
                    }
                }
            }
        }
        EDLabel {
            text: qsTr("Data bits")
        }
        EDComboBox {
            id: dataBitsComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement{text: "8"; value: 8}
                ListElement{text: "7"; value: 7}
                ListElement{text: "6"; value: 6}
                ListElement{text: "5"; value: 5}
            }
            onActivated: {
                edSettings.edSetValue(settingKeys.dataBits, dataBitsComboBox.currentValue)
            }
            Component.onCompleted: {
                var dataBit = edSettings.edValue(settingKeys.dataBits)
                if (dataBit !== 0) {
                    var ret = dataBitsComboBox.indexOfValue(dataBit)
                    if (ret !== -1) {
                        dataBitsComboBox.currentIndex = ret
                    }
                }
            }
        }
        EDLabel {
            text: qsTr("Parity")
        }
        EDComboBox {
            id: parityComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement{text: qsTr("No"); value: 0}
                ListElement{text: qsTr("Even"); value: 2}
                ListElement{text: qsTr("Odd"); value: 3}
                ListElement{text: qsTr("Space"); value: 4}
                ListElement{text: qsTr("Mark"); value: 5}
            }
            onActivated: {
                edSettings.edSetValue(settingKeys.parity, parityComboBox.currentValue)
            }
            Component.onCompleted: {
                var parity = edSettings.edValue(settingKeys.parity)
                if (parity !== 0) {
                    var ret = parityComboBox.indexOfValue(parity)
                    if (ret !== -1) {
                        parityComboBox.currentIndex = ret
                    }
                }
            }
        }
        EDLabel {
            text: qsTr("Flow control")
        }
        EDComboBox {
            id: flowControlComboBox
            valueRole: "value"
            textRole: "text"
            enabled: !devIsWorking
            Layout.fillWidth: true
            model: ListModel {
                ListElement{text: qsTr("No"); value: 0}
                ListElement{text: qsTr("Hardware"); value: 1}
                ListElement{text: qsTr("Software"); value: 2}
            }
            onActivated: {
                edSettings.edSetValue(settingKeys.flowControl,
                                      flowControlComboBox.currentValue)
            }
            Component.onCompleted: {
                var flowControl = edSettings.edValue(settingKeys.flowControl)
                if (flowControl !== 0) {
                    var ret = flowControlComboBox.indexOfValue(flowControl)
                    if (ret !== -1) {
                        flowControlComboBox.currentIndex = ret
                    }
                }
            }
        }
        EDCheckBox {
            id: bdCheckBox
            text: qsTr("Custom baud rate")
            enabled: !devIsWorking
            Layout.columnSpan: 2
            Layout.fillWidth: true
            onClicked: {
                edSettings.edSetValue(settingKeys.customBaudRate, bdCheckBox.checked)
            }
            Component.onCompleted: {
                var customBaudRate = edSettings.edValue(settingKeys.customBaudRate)
                bdCheckBox.checked = customBaudRate === edTrue ? true : false
            }
        }
        EDCheckBox {
            id: ignoredBusyDeviceCheckBox
            text: qsTr("Ignore busy device")
            enabled: !devIsWorking
            Layout.fillWidth: true
            Layout.columnSpan: 2
            onClicked: {
                spController.setIgnoredBusyDevice(ignoredBusyDeviceCheckBox.checked)
                edSettings.edSetValue(settingKeys.ignoredBusyDevice,
                                      ignoredBusyDeviceCheckBox.checked)
            }
            Component.onCompleted: {
                var ignoredBusyDevice = edSettings.edValue(settingKeys.ignoredBusyDevice)
                ignoredBusyDeviceCheckBox.checked = ignoredBusyDevice === edTrue ? true : false
            }
        }
        EDCheckBox {
            id: autoUpdatePortNameCheckBox
            text: qsTr("Auto update port names")
            enabled: !devIsWorking
            Layout.fillWidth: true
            Layout.columnSpan: 2
            onClicked: {
                spController.setAutoUpdatePortNames(autoUpdatePortNameCheckBox.checked)
                edSettings.edSetValue(settingKeys.autoUpdatePortNames, autoUpdatePortNameCheckBox.checked)
            }
            Component.onCompleted: {
                var autoUpdatePortNames = edSettings.edValue(settingKeys.autoUpdatePortNames)
                autoUpdatePortNameCheckBox.checked = autoUpdatePortNames === edTrue ? true : false
            }
        }
        Item { Layout.fillHeight: true }
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
