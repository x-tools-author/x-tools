import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import xTools.xDebug

ControllerBase {
    id: root

    property Serialport serialPort: device

    SerialPortScanner {
        id: serialPortScanner
        Component.onCompleted: {
            serialPortScanner.start()
            serialPortScanner.portNamesChanged.connect(function (portNames) {
                if (!portNameControlsComboBox.enabled) {
                    return
                }

                if (portNameControlsComboBox.popup.visible) {
                    return
                }

                if (portNameControlsComboBox.model === portNames) {
                    return
                }

                var currentText = portNameControlsComboBox.displayText
                portNameControlsComboBox.model = portNames
                var index = portNameControlsComboBox.find(currentText)
                if (index === -1) {
                    index = 0
                }

                portNameControlsComboBox.currentIndex = index
            })
        }
    }

    QtObject {
        id: settingKeys
        readonly property string portName: "portName"
        readonly property string baudRate: "baudRate"
        readonly property string dataBits: "dataBits"
        readonly property string stopBits: "stopBits"
        readonly property string parity: "parity"
        readonly property string flowControl: "flowControl"
        readonly property string ignoredBusyDevices: "ignoredBusyDevices"
        readonly property string optimizedFrame: "optimizedFrame"
        readonly property string group: "SerialPort"
    }
    Label {
        text: qsTr("Port name")
    }
    ControlsComboBox {
        id: portNameControlsComboBox
        model: serialPortScanner.portNames
        enabled: !isWorking
        onActivated: writeSettings()
        Layout.fillWidth: true
    }
    Label {
        text: qsTr("Baud rate")
    }
    ControlsComboBox {
        id: baudRateControlsComboBox
        model: serialPortScanner.baudRates()
        editable: true
        enabled: !isWorking
        onActivated: writeSettings()
        Layout.fillWidth: true
        Component.onCompleted: {
            var index = baudRateControlsComboBox.find("9600")
            baudRateControlsComboBox.currentIndex = index === -1 ? 0 : index
        }
    }
    Label {
        text: qsTr("Stop bits")
    }
    ControlsComboBox {
        id: stopBitsControlsComboBox
        valueRole: "value"
        textRole: "text"
        enabled: !isWorking
        onActivated: writeSettings()
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
    }
    Label {
        text: qsTr("Data bits")
    }
    ControlsComboBox {
        id: dataBitsControlsComboBox
        valueRole: "value"
        textRole: "text"
        enabled: !isWorking
        onActivated: writeSettings()
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
    }
    Label {
        text: qsTr("Parity")
    }
    ControlsComboBox {
        id: parityControlsComboBox
        valueRole: "value"
        textRole: "text"
        enabled: !isWorking
        onActivated: writeSettings()
        Layout.fillWidth: true
        model: ListModel {
            ListElement {
                text: qsTr("None")
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
    }
    Label {
        text: qsTr("Flow control")
    }
    ControlsComboBox {
        id: flowControlControlsComboBox
        valueRole: "value"
        textRole: "text"
        enabled: !isWorking
        onActivated: writeSettings()
        Layout.fillWidth: true
        model: ListModel {
            ListElement {
                text: qsTr("None")
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
    }
    CheckBox {
        id: ignoreBusyDevicesCheckBox
        text: qsTr("Ignore busy devices")
        onClicked: {
            serialPortScanner.setIsBusyDevicesIgnored(checked)
            writeSettings()
        }
    }
    CheckBox {
        id: optimizedFrameCheckBox
        text: qsTr("Optimized frame")
        onClicked: {
            writeSettings()
        }
    }
    Item {
        Layout.fillHeight: true
    }

    Component.onCompleted: {
        var portNameKey = settingKeys.group + "/" + settingKeys.portName
        var baudRateKey = settingKeys.group + "/" + settingKeys.baudRate
        var dataBitsKey = settingKeys.group + "/" + settingKeys.dataBits
        var stopBitsKey = settingKeys.group + "/" + settingKeys.stopBits
        var parityKey = settingKeys.group + "/" + settingKeys.parity
        var flowControlKey = settingKeys.group + "/" + settingKeys.flowControl
        var ignoreBusyDevicesKey = settingKeys.group + "/" + settingKeys.ignoreBusyDevices
        var optimizedFrameKey = settingKeys.group + "/" + settingKeys.optimizedFrame

        var portName = xSettings.value(portNameKey, "")
        var baudRate = xSettings.value(baudRateKey, 9600)
        var dataBits = xSettings.value(dataBitsKey, 8)
        var stopBits = xSettings.value(stopBitsKey, 1)
        var parity = xSettings.value(parityKey, 0)
        var flowControl = xSettings.value(flowControlKey, 0)
        var ignoreBusyDevices = xSettings.value(ignoreBusyDevicesKey, false)
        var optimizedFrame = xSettings.value(optimizedFrameKey, false)

        var index = portNameControlsComboBox.find(portName)
        if (index === -1) {
            portNameControlsComboBox.model = [portName].concat(portNameControlsComboBox.model)
            portNameControlsComboBox.currentIndex = 0
        } else {
            portNameControlsComboBox.currentIndex = index
        }
        index = baudRateControlsComboBox.find(baudRate.toString())
        baudRateControlsComboBox.currentIndex = index === -1 ? 0 : index
        index = dataBitsControlsComboBox.indexOfValue(dataBits)
        dataBitsControlsComboBox.currentIndex = index === -1 ? 0 : index
        index = dataBitsControlsComboBox.indexOfValue(stopBits)
        stopBitsControlsComboBox.currentIndex = index === -1 ? 0 : index
        index = dataBitsControlsComboBox.indexOfValue(parity)
        parityControlsComboBox.currentIndex = index === -1 ? 0 : index
        index = dataBitsControlsComboBox.indexOfValue(flowControl)
        flowControlControlsComboBox.currentIndex = index === -1 ? 0 : index
        ignoreBusyDevicesCheckBox.checked = ignoreBusyDevices
        optimizedFrameCheckBox.checked = optimizedFrame
    }

    function writeSettings() {
        var portName = portNameControlsComboBox.currentText
        var baudRate = baudRateControlsComboBox.currentText
        var dataBits = dataBitsControlsComboBox.currentValue
        var stopBits = stopBitsControlsComboBox.currentValue
        var parity = parityControlsComboBox.currentValue
        var flowControl = flowControlControlsComboBox.currentValue
        var ignoreBusyDevices = ignoreBusyDevicesCheckBox.checked

        var portNameKey = settingKeys.group + "/" + settingKeys.portName
        var baudRateKey = settingKeys.group + "/" + settingKeys.baudRate
        var dataBitsKey = settingKeys.group + "/" + settingKeys.dataBits
        var stopBitsKey = settingKeys.group + "/" + settingKeys.stopBits
        var parityKey = settingKeys.group + "/" + settingKeys.parity
        var flowControlKey = settingKeys.group + "/" + settingKeys.flowControl
        var ignoreBusyDevicesKey = settingKeys.group + "/" + settingKeys.ignoreBusyDevices
        var optimizedFrameKey = settingKeys.group + "/" + settingKeys.optimizedFrame

        xSettings.setValue(portNameKey, portName)
        xSettings.setValue(baudRateKey, parseInt(baudRate))
        xSettings.setValue(dataBitsKey, dataBits)
        xSettings.setValue(stopBitsKey, stopBits)
        xSettings.setValue(parityKey, parity)
        xSettings.setValue(flowControlKey, flowControl)
        xSettings.setValue(ignoreBusyDevicesKey, ignoreBusyDevices)
        xSettings.setValue(optimizedFrameKey, optimizedFrameCheckBox.checked)
    }

    function save() {
        var portName = portNameControlsComboBox.currentText
        var baudRate = baudRateControlsComboBox.currentText
        var dataBits = dataBitsControlsComboBox.currentValue
        var stopBits = stopBitsControlsComboBox.currentValue
        var parity = parityControlsComboBox.currentValue
        var flowControl = flowControlControlsComboBox.currentValue

        var parameters = {}
        parameters[settingKeys.portName] = portName
        parameters[settingKeys.baudRate] = parseInt(baudRate)
        parameters[settingKeys.dataBits] = dataBits
        parameters[settingKeys.stopBits] = stopBits
        parameters[settingKeys.parity] = parity
        parameters[settingKeys.flowControl] = flowControl
        parameters[settingKeys.ignoredBusyDevices] = ignoreBusyDevicesCheckBox.checked
        parameters[settingKeys.optimizedFrame] = optimizedFrameCheckBox.checked

        return parameters
    }
}
