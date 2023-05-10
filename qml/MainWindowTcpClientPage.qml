import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import ED.EasyDebug

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "TCPClient"

    property EDTcpClientTool tcpClientTool: edDevice.communication

    Component {
        id: comComtrollerComponent
        MainWindowTcpClientPageController {

        }
    }

    Component.onCompleted: {
        edDevice.setupComunicationTool(EDToolsFactory.TcpClientTool)
        if (tcpClientTool) {
            var controller = deviceControllerLoader.item
            tcpClientTool.clientIp = Qt.binding(function (){return controller.deviceController.clientIp})
            tcpClientTool.clientPort = Qt.binding(function (){return Number(controller.deviceController.clientPort)})
            tcpClientTool.specifyClientIpPort = Qt.binding(function (){return controller.deviceController.specifyClientIpPort})
            tcpClientTool.serverIp = Qt.binding(function (){return controller.deviceController.serverIp})
            tcpClientTool.serverPort = Qt.binding(function (){return Number(controller.deviceController.serverPort)})
            controller.deviceController.bindingIpPort = Qt.binding(function (){return tcpClientTool.bindingIpPort})
            controller.deviceController.deviceIsWorking = Qt.binding(function (){return tcpClientTool.isWorking})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
