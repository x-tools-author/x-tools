import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "../toolbox"

ToolBox {
    id: root
    controllerComponent: controllerComponent
    groupName: "UDPClient"

    property SAKUdpClientTool udpClientTool: null

    Component {
        id: controllerComponent
        UdpClientPageController {}
    }

    Component.onCompleted: {
        toolBox.initialize(SAKToolsFactory.UdpClientTool)
        udpClientTool = toolBox.communicator
        if (udpClientTool) {
            var controller = deviceControllerLoader.item
            udpClientTool.clientIp = Qt.binding(function () {
                return controller.deviceController.clientIp
            })
            udpClientTool.clientPort = Qt.binding(function () {
                return Number(controller.deviceController.clientPort)
            })
            udpClientTool.specifyClientIpPort = Qt.binding(function () {
                return controller.deviceController.specifyClientIpPort
            })
            udpClientTool.serverIp = Qt.binding(function () {
                return controller.deviceController.serverIp
            })
            udpClientTool.serverPort = Qt.binding(function () {
                return Number(controller.deviceController.serverPort)
            })
            controller.deviceController.bindingIpPort = Qt.binding(function () {
                return udpClientTool.bindingIpPort
            })
            controller.deviceController.deviceIsWorking = Qt.binding(function () {
                return udpClientTool.isWorking
            })
        }
    }

    onInvokeOpenDevice: toolBox.open()
    onInvokeCloseDevice: toolBox.close()
}
