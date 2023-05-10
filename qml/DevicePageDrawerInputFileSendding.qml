import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ED.EasyDebug
import "common"

EDGroupBox {
    title: qsTr("File sending")

    QtObject {
        id: settingKeys
        property string inputFileSendingFrameLength: groupName + "/inputFileSendingFrameLength"
        property string inputFileSendingFrameInterval: groupName + "/inputFileSendingFrameInterval"
        property string inputFileSendingFileName: groupName + "/inputFileSendingFileName"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        EDLabel {
            text: qsTr("Frame length(byte)")
        }
        EDSpinBox {
            settingKey: settingKeys.inputFileSendingFrameLength
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Frame interval(ms)")
        }
        EDSpinBox {
            settingKey: settingKeys.inputFileSendingFrameInterval
            Layout.fillWidth: true
        }
        EDButton {
            text: qsTr("SelectFile")
            Layout.fillWidth: true
        }
        EDButton {
            text: qsTr("StartSending");
            Layout.fillWidth: true
        }
        EDTextField {
            settingKey: settingKeys.inputFileSendingFileName
            placeholderText: qsTr("File name")
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
        EDProgressBar{
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
    }
} // EDGroupBox
