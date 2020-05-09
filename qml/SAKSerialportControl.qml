import QtQuick 2.12
import QtQuick.Layouts 1.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

GridLayout {
    id: root
    columns: 2
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    property SAKDebugger sakdebugger: null
    property SAKDebuggerDeviceSerialport device: sakdebugger ? sakdebugger.device : null
    property var baudRates: device ? device.baudRates : []
    property var stopBits: device ? device.stopBits : []
    property var paritys: device ? device.paritys : []
    property var dataBits: device ? device.dataBits : []

    SAKText {
        text: qsTr("串口选择")
        height: 25
        verticalAlignment: Text.AlignVCenter
    }

    SAKComboBox {
        id: coms
        model: sakdebugger ? (device ? device.serialports : []) : []
        Layout.fillWidth: true
        customEnable: device ? !device.deviceIsOpened : false

        onCurrentTextChanged: {
            if (device){
                device.paraPortName  = currentText
            }
        }
    }

    SAKCheckBox {
        id: sakCheckBox
        text: qsTr("自定义波特率")
        Layout.columnSpan: 2
        enabled: device ? !device.deviceIsOpened : false

        onCheckedChanged: {
            comboBoxRepeater.itemAt(0).visible = !checked
            textRepeater.itemAt(0).visible = !checked

            if (device){
                if (checked){
                    device.paraBaudRate = customBd.text
                }else{
                    device.paraBaudRate = comboBoxRepeater.itemAt(0).currentText
                }
            }
        }
    }

    SAKText {
        text: qsTr("波特率")
        Layout.column: 0
        Layout.row: index+2
        visible: sakCheckBox.checked
    }

    SAKLineEdit {
        id: customBd
        visible: sakCheckBox.checked
        Layout.fillWidth: true
        readOnly: device ? device.deviceIsOpened : false

        onTextChanged: {
            if (device){
                if (sakCheckBox.checked){
                    device.paraBaudRate = text
                }
            }
        }
    }

    Repeater {
        id: textRepeater
        model: [qsTr("波特率"), qsTr("数据位"), qsTr("停止位"), qsTr("奇偶位")]

        SAKText {
            text: modelData
            Layout.column: 0
            Layout.row: index+3
        }
    }

    Repeater {
        id: comboBoxRepeater
        model: 4
        SAKComboBox {
            model: index == 0 ? baudRates : (index == 1 ? dataBits : (index == 2 ? stopBits : (index == 3 ? paritys : [])))
            Layout.column: 1
            Layout.row: index+3
            Layout.fillWidth: true
            customEnable: device ? !device.deviceIsOpened : false

            onCurrentTextChanged: {
                if (device){
                    switch (index){
                    case 0:
                        device.paraBaudRate = currentText
                        break;
                    case 1:
                        device.paraDataBits = currentText
                        break;
                    case 2:
                        device.paraStopBits = currentText
                        break;
                    case 3:
                        device.paraParity = currentText
                        break;
                    default:
                        console.info("Unknow type")
                        break;
                    }
                }
            }

            onModelChanged: {
                if (index == 0){
                    var ret = find("9600")
                    if (ret > 0){
                        currentIndex = ret
                    }
                }
                if (index == 1){
                    ret = find("Data8")
                    if (ret > 0){
                        currentIndex = ret
                    }
                }
            }
        }
    }

    Repeater {
        id: deviceButtonRepeater
        model: [
            [qsTr("刷新设备"), qsTr("刷新设备")],
            [qsTr("打开设备"), qsTr("关闭设备")]
        ]

        SAKButton {
            id: bt
            text: index == 1 ? (device ? (device.deviceIsOpened ? modelData[1] : modelData[0]) : modelData[0]) : modelData[0]
            Layout.fillWidth: true
            customEnable: index == 0 ? (device ? !device.deviceIsOpened : false) : true

            onClicked: {
                if (customEnable){
                    if (index == 0){
                        if (device){
                            device.refresh()
                        }
                    }else{
                        if (device){
                            device.open()
                        }
                    }
                }
            }

            Connections {
                target: device
                onDeviceIsOpenedChanged: {
                    if (index == 1){
                        bt.color = device.deviceIsOpened ? "#FF0000" : "#FFFFFF"
                    }
                }
            }
        }
    }
}
