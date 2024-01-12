import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import SAK.Custom
import "../toolbox"

ToolBox {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "BleCentral"

    property SAKBleCentralTool bleCentral: toolBox.communication ? toolBox.communication : null

    Component {
        id: comComtrollerComponent
        BleCentralPageController {
            onInvokeChangedNotify: root.onInvokeChangedNotify()
            onInvokeRead: root.onInvokeRead()
            onCurrentCharacteristicIndexChanged: updateNotify()
        }
    }

    Connections {
        target: bleCentral
        function onDescriptorWritten(descriptor, newValue) {
            updateNotify(newValue)
        }
    }

    Component.onCompleted: {
        toolBox.initialize(SAKToolsFactory.BleCentral)
        if (bleCentral) {
            var controller = deviceControllerLoader.item
            bleCentral.currentCharacteristicIndex = Qt.binding(function () {
                return controller.currentCharacteristicIndex
            })
            bleCentral.currentServiceIndex = Qt.binding(function () {
                return controller.currentServiceIndex
            })
            bleCentral.writeModel = Qt.binding(function () {
                return controller.writeModel
            })
            controller.characteristicNames = Qt.binding(function () {
                return bleCentral.characteristicNames
            })
            controller.serviceNames = Qt.binding(function () {
                return bleCentral.serviceNames
            })
        }
    }

    onInvokeOpenDevice: {
        toolBox.open()
    }

    onInvokeCloseDevice: {
        toolBox.close()
    }

    function onInvokeChangedNotify() {
        bleCentral.changeNotify()
    }

    function onInvokeRead() {
        bleCentral.readCharacteristic()
    }

    function updateNotify(value) {
        var cookedValue = sakInterface.arrayToString(value, sakDataStructure.TextFormatHex)
        var controller = deviceControllerLoader.item
        if (cookedValue === "00 00") {
            controller.setNotifyText(false)
        } else {
            controller.setNotifyText(true)
        }
    }
}
