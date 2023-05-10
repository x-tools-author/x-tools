import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import ED.EasyDebug

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "SerialPort"

    property EDSerialportTool comTool: edDevice.communication

    Component {
        id: comComtrollerComponent
        MainWindowSerialPortPageController {
            comTool: root.edDevice.communication
        }
    }

    Component.onCompleted: {
        edDevice.setupComunicationTool(EDToolsFactory.SerialportTool)
    }

    onInvokeOpenDevice: {
        var comComtroller = deviceControllerLoader.item
        var portName = comComtroller.portName()
        var baudRate = comComtroller.baudRate()
        var dataBits = comComtroller.dataBits()
        var stopBits = comComtroller.stopBits()
        var flowControl = comComtroller.flowControl()

        comTool.setPortName(portName)
        comTool.setBaudRate(baudRate)
        comTool.setDataBits(dataBits)
        comTool.setStopBits(stopBits)
        comTool.setFlowControl(flowControl)
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
