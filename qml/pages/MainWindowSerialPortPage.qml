import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "SerialportToolBox"

    property SAKSerialportTool comTool: null

    Component {
        id: comComtrollerComponent
        MainWindowSerialPortPageController {
            comTool: root.edDevice.communicaton
        }
    }

    Component.onCompleted: {
        edDevice.initialize(SAKToolsFactory.SerialportTool)
        comTool = edDevice.communicaton
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
