import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "UDPServer"

    property SAKUdpServerTool udpServerTool: edDevice.communication ? edDevice.communication : null

    Component {
        id: comComtrollerComponent
        MainWindowUdpServerPageController {

        }
    }

    Component.onCompleted: {
        edDevice.initialize(SAKToolsFactory.UdpServerTool)
        if (udpServerTool) {
            var controller = deviceControllerLoader.item
            udpServerTool.serverIp = Qt.binding(function (){return controller.deviceController.serverIp})
            udpServerTool.serverPort = Qt.binding(function (){return Number(controller.deviceController.serverPort)})
            udpServerTool.clientIndex = Qt.binding(function (){return Number(controller.deviceController.clientIndex)})
            controller.deviceController.deviceIsWorking = Qt.binding(function (){return udpServerTool.isWorking})
            controller.deviceController.clients = Qt.binding(function (){return udpServerTool.clients})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
