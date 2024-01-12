import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "../toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "TcpServer"

    property SAKTcpServerTool tcpServerTool: nullptr

    Component {
        id: comComtrollerComponent
        TcpServerPageController {}
    }

    Component.onCompleted: {
        toolBox.initialize(SAKToolsFactory.TcpServerTool)
        tcpServerTool = toolBox.communicator
        if (tcpServerTool) {
            var controller = deviceControllerLoader.item
            tcpServerTool.serverIp = Qt.binding(function () {
                return controller.deviceController.serverIp
            })
            tcpServerTool.serverPort = Qt.binding(function () {
                return Number(controller.deviceController.serverPort)
            })
            tcpServerTool.clientIndex = Qt.binding(function () {
                return Number(controller.deviceController.clientIndex)
            })
            controller.deviceController.deviceIsWorking = Qt.binding(function () {
                return tcpServerTool.isWorking
            })
            controller.deviceController.clients = Qt.binding(function () {
                return tcpServerTool.clients
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
