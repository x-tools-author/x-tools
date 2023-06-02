import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../common"

SAKPopup {
    id: root
    background: SAKPane{}

    property alias txMasker: devicePageDrawerDevice.txMasker
    property alias rxMasker: devicePageDrawerDevice.rxMasker
    property alias analyzer: devicePageDrawerDevice.analyzer
    property alias storer: devicePageDrawerOutput.storer

    property alias showWrap: devicePageDrawerOutput.showWrap
    property alias showDate: devicePageDrawerOutput.showDate
    property alias showTime: devicePageDrawerOutput.showTime
    property alias showMs: devicePageDrawerOutput.showMs
    property alias showTx: devicePageDrawerOutput.showTx
    property alias showRx: devicePageDrawerOutput.showRx

    property alias prefix: devicePageDrawerIntput.prefix
    property alias suffix: devicePageDrawerIntput.suffix
    property alias escapeCharacter: devicePageDrawerIntput.escapeCharacter

    property alias crcType: devicePageDrawerIntput.crcType
    property alias crcStartIndex: devicePageDrawerIntput.crcStartIndex
    property alias crcEndIndex: devicePageDrawerIntput.crcEndIndex
    property alias crcEnable: devicePageDrawerIntput.crcEnable
    property int offset: 0
    property alias inputEscapeCharacter: devicePageDrawerIntput.escapeCharacter

    contentItem: ColumnLayout {
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            Repeater {
                model: [qsTr("Device"), qsTr("Output"), qsTr("Input")]
                TabButton {
                    text: modelData
                    Layout.fillWidth: true
                }
            }
        }
        StackLayout {
            currentIndex: tabBar.currentIndex
            ToolBoxDrawerDevice {
                id: devicePageDrawerDevice
            }
            ToolBoxDrawerOutput {
                id: devicePageDrawerOutput
            }
            ToolBoxDrawerInput {
                id: devicePageDrawerIntput
            }
        }
    }

    onOffsetChanged: {
        devicePageDrawer.x = mainWindow.width - mainWindowPane.anchors.margins - offset
    }

    NumberAnimation {
        id: drawerAnimation
        from: 0
        to: devicePageDrawer.width
        duration: 256
        target: devicePageDrawer
        properties: "offset"
    }

    Overlay.modal: Rectangle {
        color: "#00000000"
        Rectangle {
            radius: 8
            anchors.fill: parent
            anchors.leftMargin: mainWindowPane.anchors.margins
            anchors.rightMargin: mainWindowPane.anchors.margins
            anchors.topMargin: mainWindowPane.anchors.margins + titleBar.height
            anchors.bottomMargin: mainWindowPane.anchors.margins
            color: Material.backgroundDimColor
        }
    }

    function customOpen(pageIndex) {
        tabBar.currentIndex = pageIndex
        devicePageDrawer.x = mainWindow.width - mainWindowPane.anchors.margins
        open()
        drawerAnimation.start()
    }

    function setDocument(doc) {
        devicePageDrawerOutput.setDocument(doc)
    }
}
