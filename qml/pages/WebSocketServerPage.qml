import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "../toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "WebSocketServer"

    property SAKWebSocketServerTool webSocketServer: toolBox.communication ? toolBox.communication : null

    Component {
        id: comComtrollerComponent
        WebSocketServerPageController {}
    }

    Component.onCompleted: {
        toolBox.initialize(SAKToolsFactory.WebSocketServerTool)
        if (webSocketServer) {
            var controller = deviceControllerLoader.item
            webSocketServer.serverIp = Qt.binding(function () {
                return controller.deviceController.serverIp
            })
            webSocketServer.serverPort = Qt.binding(function () {
                return Number(controller.deviceController.serverPort)
            })
            webSocketServer.clientIndex = Qt.binding(function () {
                return Number(controller.deviceController.clientIndex)
            })
            webSocketServer.messageType = Qt.binding(function () {
                return Number(controller.deviceController.messageType)
            })
            controller.deviceController.deviceIsWorking = Qt.binding(function () {
                return webSocketServer.isWorking
            })
            controller.deviceController.clients = Qt.binding(function () {
                return webSocketServer.clients
            })
        }
    }

    onInvokeOpenDevice: {
        toolBox.open()
    }

    onInvokeCloseDevice: {
        toolBox.close()
    }
}
