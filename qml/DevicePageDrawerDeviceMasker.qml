import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ED.EasyDebug
import "common"

EDGroupBox {
    id: maskerGroupBox
    title: qsTr("Masker")
    padding: 8

    property EDMaskerTool rxMasker: null
    property EDMaskerTool txMasker: null

    QtObject {
        id: settingKeys
        readonly property string maskerRxEnable: "maskerRxEnable"
        readonly property string maskerRxMask: "maskerRxMask"
        readonly property string maskerTxEnable: "maskerTxEnable"
        readonly property string maskerTxMask: "maskerTxMask"
    }

    EDHexRegularExpressionValidator{ id:hexREV }

    GridLayout {
        columns: 2
        anchors.fill: parent

        EDCheckBox {
            id: rxMaskerCheckBox
            text: qsTr("Enable reading masker")
            settingKey: settingKeys.maskerRxEnable
            Layout.columnSpan: 2
            onClicked: setMaskerEnable(rxMaskerCheckBox.checked, rxMasker)
            Component.onCompleted: setMaskerEnable(rxMaskerCheckBox.checked, rxMasker)
        }

        EDCheckBox {
            id: txMaskerCheckBox
            text: qsTr("Enable writing masker")
            settingKey: settingKeys.maskerTxEnable
            Layout.columnSpan: 2
            onClicked: setMaskerEnable(txMaskerCheckBox.checked, txMasker)
            Component.onCompleted: setMaskerEnable(txMaskerCheckBox.checked, txMasker)
        }

        EDLabel {
            text: qsTr("Reading mask")
        }

        EDTextField {
            id: rxMaskerField
            validator: hexREV
            maximumLength: 2
            settingKey: settingKeys.maskerRxMask
            Layout.fillWidth: true
            onTextEdited: setMaskerMask(rxMaskerField.text, rxMasker)
            Component.onCompleted: setMaskerMask(rxMaskerField.text, rxMasker)
        }

        EDLabel {
            text: qsTr("Writing mask")
        }
        EDTextField {
            id: txMaskerField
            validator: hexREV
            maximumLength: 2
            settingKey: settingKeys.maskerTxMask
            Layout.fillWidth: true
            onTextEdited: setMaskerMask(txMaskerField.text, txMasker)
            Component.onCompleted: setMaskerMask(txMaskerField.text, txMasker)
        }
    }

    function setMaskerEnable(enable, masker) {
        if (masker) {
            masker.enable = enable
        }
    }

    function setMaskerMask(mask, masker) {
        if (masker) {
            var cookedMask = parseInt(mask, 16)
            masker.setMaskCode(cookedMask)
        }
    }
}
