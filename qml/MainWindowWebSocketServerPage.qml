import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import ED.EasyDebug

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "WebSocketServer"

    property SAKWebSocketServerTool webSocketServer: edDevice.communication ? edDevice.communication : null

    Component {
        id: comComtrollerComponent
        MainWindowWebSocketServerPageController {

        }
    }

    Component.onCompleted: {
        edDevice.initialize(EDToolsFactory.WebSocketServerTool)
        if (webSocketServer) {
            var controller = deviceControllerLoader.item
            webSocketServer.serverIp = Qt.binding(function (){return controller.deviceController.serverIp})
            webSocketServer.serverPort = Qt.binding(function (){return Number(controller.deviceController.serverPort)})
            webSocketServer.clientIndex = Qt.binding(function (){return Number(controller.deviceController.clientIndex)})
            webSocketServer.messageType = Qt.binding(function (){return Number(controller.deviceController.messageType)})
            controller.deviceController.deviceIsWorking = Qt.binding(function (){return webSocketServer.isWorking})
            controller.deviceController.clients = Qt.binding(function (){return webSocketServer.clients})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
