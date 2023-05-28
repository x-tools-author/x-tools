import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ED.EasyDebug
import "common"

Item {
    id: root

    property alias edDevice: edDevice
    property alias controllerModule: devicePageController
    property alias readwriteModule: devicePageReadWrite
    property alias drawerModule: devicePageReadWrite
    property alias deviceControllerLoader: devicePageController.deviceControllerLoader
    property var controllerComponent: null
    property string groupName: "EasyDebug"

    signal invokeOpenDevice()
    signal invokeCloseDevice()

    EDDevice {
        id: edDevice
    }

    DevicePageController {
        id: devicePageController
        controllerComponent: root.controllerComponent
        communicationTool: edDevice.communication
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
        EDVerticalLine { anchors.right: parent.right }
    }

    DevicePageReadWrite {
        id: devicePageReadWrite
        communicationTool: edDevice.communication
        emitterTool: edDevice.emitter
        analyzer: edDevice.analyzer
        responserTool: edDevice.responserTool()
        presotrerTool: edDevice.prestorerTool()
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
            if (!edDevice.isWorking) {
                console.info("The device is not working, the operation will be ignored!")
                return
            }

            var text = devicePageReadWrite.inputText
            if (text === "") {
                console.info("The input text if empty, the operaion will be ignored!")
                return
            }

            var cookedString = sakDataStructure.cookEscapeCharacter(devicePageDrawer.inputEscapeCharacter, text)
            var cookedBytes = sakInterface.string2array(cookedString, inputFormat)
            cookedBytes = sakInterface.arrayAppendArray(devicePageDrawer.prefix, cookedBytes)
            if (devicePageDrawer.crcEnable) {
                var startIndex = devicePageDrawer.crcStartIndex
                var endIndex = devicePageDrawer.crcEndIndex
                var model = devicePageDrawer.crcType
                var crcBytes = sakCrc.calculateBytes(cookedBytes, model, startIndex, endIndex)
                cookedBytes = sakInterface.arrayAppendArray(cookedBytes, crcBytes);
            }
            cookedBytes = sakInterface.arrayAppendArray(cookedBytes, devicePageDrawer.suffix)
            edDevice.send(cookedBytes)

            appendHisroty(text, inputFormat)
        }
    }

    DevicePageDrawer {
        id: devicePageDrawer
        width: 350
        x: mainWindow.width - mainWindowPane.anchors.margins
        y: titleBar.height + mainWindowPane.anchors.margins
        height: root.height
        txMasker: edDevice.inputMasker
        rxMasker: edDevice.outputMasker
        analyzer: edDevice.analyzer
        storer: edDevice.storer
        Component.onCompleted: setDocument(devicePageReadWrite.textVar)
    }
}
