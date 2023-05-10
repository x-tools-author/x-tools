import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import ED.EasyDebug

DevicePage {
    id: root
    controllerComponent: comComtrollerComponent
    groupName: "BleCentral"

    property EDBleCentralTool bleCentral: edDevice.communication

    Component {
        id: comComtrollerComponent
        MainWindowBleCentralPageController {
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
        edDevice.setupComunicationTool(EDToolsFactory.BleCentral)
        if (bleCentral) {
            var controller = deviceControllerLoader.item
            bleCentral.currentCharacteristicIndex = Qt.binding(function (){return controller.currentCharacteristicIndex})
            bleCentral.currentServiceIndex = Qt.binding(function (){return controller.currentServiceIndex})
            bleCentral.writeModel = Qt.binding(function (){return controller.writeModel})
            controller.characteristicNames = Qt.binding(function (){return bleCentral.characteristicNames})
            controller.serviceNames = Qt.binding(function (){return bleCentral.serviceNames})
        }
    }

    onInvokeOpenDevice: {
        edDevice.open()
    }

    onInvokeCloseDevice: {
        edDevice.close()
    }

    function onInvokeChangedNotify() {
        bleCentral.changeNotify()
    }

    function onInvokeRead() {
        bleCentral.readCharacteristic()
    }

    function updateNotify(value) {
        var cookedValue = edInterface.array2String(value, edDataStructure.TextFormatHex)
        var controller = deviceControllerLoader.item
        if (cookedValue === "00 00") {
            controller.setNotifyText(false)
        } else {
            controller.setNotifyText(true)
        }
    }
}
