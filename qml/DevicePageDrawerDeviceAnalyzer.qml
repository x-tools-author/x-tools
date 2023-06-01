import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

SAKGroupBox {
    id: analyzerGroupBox
    title: qsTr("Analyzer")

    property EDAnalyzerTool analyzer: null

    QtObject {
        id: settingKeys

        readonly property string analyzerEnable: "analyzerEnable"
        readonly property string analyzerFrameLengthIsFixed: "analyzerFrameLengthIsFixed"
        readonly property string analyzerFrameLength: "analyzerFrameLength"
        readonly property string analyzerFrameSeparationMark: "analyzerFrameSeparationMark"
        readonly property string analyzerMaxTempBytes: "analyzerMaxTempBytes"
    }

    SAKHexRegularExpressionValidator{ id:hexREV }

    GridLayout {
        columns: 2
        anchors.fill: parent
        SAKCheckBox {
            id: enableCheckBox
            text: qsTr("Enable frame analyzer")
            settingKey: settingKeys.analyzerEnable
            Layout.columnSpan: 2
            Layout.fillWidth: true
            onClicked: setAnalyzerEnable(enableCheckBox.checked)
            Component.onCompleted: setAnalyzerEnable(enableCheckBox.checked)

            function setAnalyzerEnable(enable) {
                if (analyzer) {
                    analyzer.enable = enable
                }
            }
        }
        SAKCheckBox {
            id: fixsakCheckBox
            text: qsTr("Fixed frame length")
            settingKey: settingKeys.analyzerFrameLengthIsFixed
            Layout.columnSpan: 2
            Layout.fillWidth: true
            onClicked: setAnalyzerFixed(fixsakCheckBox.checked)
            Component.onCompleted: setAnalyzerFixed(fixsakCheckBox.checked)

            function setAnalyzerFixed(fixed) {
                if (analyzer) {
                    analyzer.setFixed(fixed)
                }
            }
        }
        SAKLabel {
            text: qsTr("Frame bytes")
        }
        SAKTextField {
            id: frameBytesTextField
            settingKey: settingKeys.analyzerFrameLength
            Layout.fillWidth: true
            maximumLength: 4
            onTextEdited: setAnalyzerFrameBytes(frameBytesTextField.text)
            Component.onCompleted: setAnalyzerFrameBytes(frameBytesTextField.text)

            function setAnalyzerFrameBytes(frameBytes) {
                if (analyzer) {
                    analyzer.setFrameBytes(parseInt(frameBytes))
                }
            }
        }
        SAKLabel {
            text: qsTr("Separation mark")
        }
        SAKTextField {
            id: separationMarkTextField
            settingKey: settingKeys.analyzerFrameSeparationMark
            validator: hexREV
            Layout.fillWidth: true
            onTextEdited: setAnalyzerSeparationMark(separationMarkTextField.text)
            Component.onCompleted: setAnalyzerSeparationMark(separationMarkTextField.text)

            function setAnalyzerSeparationMark(mark) {
                if (analyzer) {
                    var cookedMark = sakInterface.string2array(
                                separationMarkTextField.text,
                                EDDataStructure.TextFormatHex)
                    analyzer.setSeparationMark(cookedMark)
                }
            }
        }
        SAKLabel {
            text: qsTr("Max temp bytes")
        }
        SAKSpinBox {
            id: maxTempBytesTextField
            settingKey: settingKeys.analyzerMaxTempBytes
            value: 1024
            editable: true
            from: 1
            to: 10240
            Layout.fillWidth: true

            function setAnalyzerMaxTempBytes(maxBytes) {
                if (analyzer) {
                    var cookedMaxBytes = parseInt(maxBytes)
                    analyzer.setMaxTempBytes(cookedMaxBytes)
                }
            }
        }
    }
}
