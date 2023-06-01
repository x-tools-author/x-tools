import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

Item {
    id: root
    //padding: 0

    property alias communicationTool: ioPage.communicationTool
    property alias emitterTool: emitterPage.emitterTool
    property alias responserTool: responserPage.responserTool
    property alias presotrerTool: prestorePage.presotrerTool
    property alias outputFormat: ioPage.outputFormat
    property alias inputFormat: ioPage.inputFormat
    property alias inputText: ioPage.inputText
    property alias analyzer: ioPage.analyzer

    property alias showWrap: ioPage.showWrap
    property alias showDate: ioPage.showDate
    property alias showTime: ioPage.showTime
    property alias showMs: ioPage.showMs
    property alias showTx: ioPage.showTx
    property alias showRx: ioPage.showRx
    property alias textVar: ioPage.textVar

    signal invokeOpenDrawer(var pageIndex)
    signal invokeSend()

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        TabBar {
            id: pageTabBar
            Layout.fillWidth: true
            Repeater {
                model: [qsTr("I/O"), qsTr("Emitter"), qsTr("Responser"), qsTr("PresetData")]
                TabButton {
                    text: modelData
                }
            }
        }
        SAKHorizontalLine {
            autoScale: false
            Layout.fillWidth: true
        }
        StackLayout {
            id: pageStackLayout
            currentIndex: pageTabBar.currentIndex
            Layout.fillHeight: true
            Layout.fillWidth: true
            DevicePageReadWriteInputOutput {
                id: ioPage
                Layout.fillHeight: true
                Layout.fillWidth: true
                onInvokeSend: root.invokeSend()
                onInvokeOpenDrawer: function func(pageIndex) {
                    root.invokeOpenDrawer(pageIndex)
                }
            }
            DevicePageReadWriteEmitter {
                id: emitterPage
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            DevicePageReadWriteResponser {
                id: responserPage
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            DevicePageReadWritePrestorer {
                id: prestorePage
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }

    function clearOutput() {
        ioPage.clearOutput()
    }

    function appendHisroty (text, format) {
        ioPage.appendHisroty(text, format)
    }
}
