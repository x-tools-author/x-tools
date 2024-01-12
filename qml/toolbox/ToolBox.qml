import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "../common"

Item {
    id: root

    property alias toolBox: innerToolBox
    property alias controllerModule: devicePageController
    property alias readwriteModule: devicePageReadWrite
    property alias drawerModule: devicePageReadWrite
    property alias deviceControllerLoader: devicePageController.deviceControllerLoader

    property var controllerComponent: null
    property string groupName: "EasyDebug"
    property SAKCommunicator communicator: innerToolBox.communicator

    signal invokeOpenDevice
    signal invokeCloseDevice

    SAKToolBox {
        id: innerToolBox
    }

    ToolBoxController {
        id: devicePageController
        controllerComponent: root.controllerComponent
        communicator: root.communicator
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 250
        onInvokeOpenDevice: root.invokeOpenDevice()
        onInvokeCloseDevice: root.invokeCloseDevice()
        onInvokeClearOutput: devicePageReadWrite.clearOutput()
        onInvokeOpenDrawer: function func(pageIndex) {
            devicePageDrawer.customOpen(pageIndex)
        }
        SAKVerticalLine {
            anchors.right: parent.right
        }
    }

    ToolBoxReadWrite {
        id: devicePageReadWrite
        communicator: root.communicator
        emitterTool: toolBox.emitter
        responserTool: toolBox.responser
        presotrerTool: toolBox.prestorer
        showWrap: devicePageDrawer.showWrap
        showDate: devicePageDrawer.showDate
        showTime: devicePageDrawer.showTime
        showMs: devicePageDrawer.showMs
        showRx: devicePageDrawer.showRx
        showTx: devicePageDrawer.showTx
        anchors.left: devicePageController.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onInvokeOpenDrawer: function func(pageIndex) {
            devicePageDrawer.customOpen(pageIndex)
        }
        onInvokeSend: function () {
            if (!toolBox.isWorking) {
                console.info("The device is not working, the operation will be ignored!")
                return
            }

            var text = devicePageReadWrite.inputText
            if (text === "") {
                console.info("The input text if empty, the operation will be ignored!")
                return
            }

            var cookedString = sakDataStructure.cookEscapeCharacter(
                        devicePageDrawer.inputEscapeCharacter, text)
            var cookedBytes = sakInterface.string2array(cookedString, inputFormat)
            cookedBytes = sakInterface.arrayAppendArray(devicePageDrawer.prefix, cookedBytes)
            if (devicePageDrawer.crcEnable) {
                var startIndex = devicePageDrawer.crcStartIndex
                var endIndex = devicePageDrawer.crcEndIndex
                var model = devicePageDrawer.crcType
                var crcBytes = sakCrc.calculateBytes(cookedBytes, model, startIndex, endIndex)
                cookedBytes = sakInterface.arrayAppendArray(cookedBytes, crcBytes)
            }
            cookedBytes = sakInterface.arrayAppendArray(cookedBytes, devicePageDrawer.suffix)
            toolBox.send(cookedBytes)

            appendHisroty(text, inputFormat)
        }
    }

    ToolBoxDrawer {
        id: devicePageDrawer
        width: 350
        x: mainWindow.width - mainWindowPane.anchors.margins
        y: titleBar.height + mainWindowPane.anchors.margins
        height: root.height
        storer: toolBox.storer
        Component.onCompleted: setDocument(devicePageReadWrite.textVar)
    }
}
