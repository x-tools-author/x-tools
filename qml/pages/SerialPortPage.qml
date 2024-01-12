import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "../toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "SerialportToolBox"

    property SAKSerialport comTool: null

    Component {
        id: comComtrollerComponent
        SerialPortPageController {
            comTool: root.toolBox.communicator
        }
    }

    Component.onCompleted: {
        toolBox.initialize(SAKToolsFactory.SerialportTool)
        comTool = toolBox.communicator
    }

    onInvokeOpenDevice: {
        var comComtroller = deviceControllerLoader.item
        var portName = comComtroller.portName()
        var baudRate = comComtroller.baudRate()
        var dataBits = comComtroller.dataBits()
        var stopBits = comComtroller.stopBits()
        var flowControl = comComtroller.flowControl()
        var parity = comComtroller.parity()

        comTool.portName = portName
        comTool.baudRate = baudRate
        comTool.dataBits = dataBits
        comTool.stopBits = stopBits
        comTool.parity = parity
        comTool.flowControl = flowControl
        toolBox.open()
    }

    onInvokeCloseDevice: {
        toolBox.close()
    }
}
