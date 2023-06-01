import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

SAKGroupBox {
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
        SAKLabel {
            text: qsTr("Frame length(byte)")
        }
        SAKSpinBox {
            settingKey: settingKeys.inputFileSendingFrameLength
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("Frame interval(ms)")
        }
        SAKSpinBox {
            settingKey: settingKeys.inputFileSendingFrameInterval
            Layout.fillWidth: true
        }
        SAKButton {
            text: qsTr("SelectFile")
            Layout.fillWidth: true
        }
        SAKButton {
            text: qsTr("StartSending");
            Layout.fillWidth: true
        }
        SAKTextField {
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
} // SAKGroupBox
