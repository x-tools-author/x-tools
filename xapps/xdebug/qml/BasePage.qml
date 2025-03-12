import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import xTools.xDebug

Item {
    id: root

    property alias controllerSource: pageController.controllerSource
    property Device device: null
    property bool isWorking: false
    required property string settingsGroup

    onDeviceChanged: {
        if (device) {
            device.opened.connect(function () {
                isWorking = true
            })

            device.finished.connect(function () {
                isWorking = false
            })
        }
    }

    GridLayout {
        columns: 2
        rowSpacing: 0
        columnSpacing: 0
        anchors.fill: parent
        BasePageController {
            id: pageController
            device: root.device
            Layout.rowSpan: 2
            Layout.minimumWidth: 200
            Layout.fillHeight: true
        }
        BasePageOutput {
            id: pageOutput
            settingsGroup: root.settingsGroup
            Layout.fillWidth: true
            Layout.fillHeight: true
            CommonSeparator {
                edge: Qt.BottomEdge
            }
        }
        BasePageInput {
            id: pageInput
            settingsGroup: root.settingsGroup
            Layout.fillWidth: true
        }
    }

    function formatMenuContext() {
        var tmp = []
        tmp.push(["Bin", xBin])
        tmp.push(["Oct", xOct])
        tmp.push(["Dec", xDec])
        tmp.push(["Hex", xHex])
        tmp.push(["Ascii", xAscii])
        tmp.push(["Utf8", xUtf8])
        return tmp
    }

    function affixesMenuContext() {
        var tmp = []
        tmp.push([xNoneText, xAffixesNone])
        tmp.push(["\\n", xAffixesN])
        tmp.push(["\\r", xAffixesR])
        tmp.push(["\\n\\r", xAffixesNR])
        tmp.push(["\\r\\n", xAffixesRN])

        return tmp
    }
}
