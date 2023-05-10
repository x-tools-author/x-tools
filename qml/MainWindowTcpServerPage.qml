import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import ED.EasyDebug

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "TcpServer"

    property EDTcpServerTool tcpServerTool: edDevice.communication

    Component {
        id: comComtrollerComponent
        MainWindowTcpServerPageController {

        }
    }

    Component.onCompleted: {
        edDevice.setupComunicationTool(EDToolsFactory.TcpServerTool)
        if (tcpServerTool) {
            var controller = deviceControllerLoader.item
            tcpServerTool.serverIp = Qt.binding(function (){return controller.deviceController.serverIp})
            tcpServerTool.serverPort = Qt.binding(function (){return Number(controller.deviceController.serverPort)})
            tcpServerTool.clientIndex = Qt.binding(function (){return Number(controller.deviceController.clientIndex)})
            controller.deviceController.deviceIsWorking = Qt.binding(function (){return tcpServerTool.isWorking})
            controller.deviceController.clients = Qt.binding(function (){return tcpServerTool.clients})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
