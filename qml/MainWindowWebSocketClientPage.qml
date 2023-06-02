import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "toolbox"

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "WebSocketClient"

    property EDWebSocketClientTool webSocketClient: edDevice.communication ? edDevice.communication : null

    Component {
        id: comComtrollerComponent
        MainWindowWebSocketClientPageController {

        }
    }

    Component.onCompleted: {
        edDevice.initialize(EDToolsFactory.WebSocketClientTool)
        if (webSocketClient) {
            var controller = deviceControllerLoader.item
            webSocketClient.clientIp = Qt.binding(function (){return controller.deviceController.clientIp})
            webSocketClient.clientPort = Qt.binding(function (){return Number(controller.deviceController.clientPort)})
            webSocketClient.specifyClientIpPort = Qt.binding(function (){return controller.deviceController.specifyClientIpPort})
            webSocketClient.serverIp = Qt.binding(function (){return controller.deviceController.serverIp})
            webSocketClient.serverPort = Qt.binding(function (){return Number(controller.deviceController.serverPort)})
            webSocketClient.messageType = Qt.binding(function (){return Number(controller.deviceController.messageType)})
            controller.deviceController.bindingIpPort = Qt.binding(function (){return webSocketClient.bindingIpPort})
            controller.deviceController.deviceIsWorking = Qt.binding(function (){return webSocketClient.isWorking})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }
}
