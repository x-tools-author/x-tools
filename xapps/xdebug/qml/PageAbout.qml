import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import xTools.xDebug

Item {
    id: root
    GridLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 4
        anchors.topMargin: 4
        columns: 2
        Label {
            text: qsTr("Version:")
        }
        Label {
            text: xApp.version()
        }
        Label {
            text: qsTr("Report a bug:")
        }
        Label {
            text: "x-tools@outlook.com"
        }
        Label {
            text: qsTr("Build info:")
        }
        // Label {
        //     text: {
        //         var format = xApp.systemDateFormat()
        //         format += " "
        //         format += xApp.systemTimeFormat()
        //         return xApp.dateTimeString(format)
        //     }
        // }
        // Label {
        //     text: qsTr("User QQ Group:")
        // }
        // Label {
        //     id: qqGroupLabel
        //     text: "952218522 " + qsTr("(Doublue click to copy)")
        //     MouseArea {
        //         anchors.fill: parent
        //         onClicked: xApp.setClipboardText("952218522")
        //     }
        // }
        // Label {
        //     text: qsTr("Copyright information:")
        // }
        // Label {
        //     text: {
        //         var yyyy = xApp.dateTimeString("yyyy")
        //         return qsTr("Copyright 2023-%1 x-tools-author. All rights reserved.").arg(yyyy)
        //     }
        // }
        // Label {
        //     text: qsTr("Last commit(xTools):")
        // }
        // Label {
        //     text: xApp.lastCommit()
        // }
        // Label {
        //     text: qsTr("Last commit time(xTools):")
        // }
        // Label {
        //     text: xApp.lastCommitTime()
        // }
        // Label {
        //     text: qsTr("Last commit(xDebug):")
        // }
        // Label {
        //     text: xDebugCommit
        // }
        // Label {
        //     text: qsTr("Last commit time(xDebug):")
        // }
        // Label {
        //     text: xDebugCommitTime
        // }

        // Item {
        //     Layout.fillHeight: true
        // }
    }

    Image {
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        source: "https://img.shields.io/badge/xTools-Source-blue"
    }
}
