import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "common"

SAKGroupBox {
    id: root
    title: qsTr("Preprocessor")
    Layout.fillWidth: true

    property bool showWrap: true
    property bool showDate: true
    property bool showTime: true
    property bool showMs: true
    property bool showTx: true
    property bool showRx: true

    QtObject {
        id: settingKeys
        readonly property string outputShowRx: groupName + "/outputShowRx"
        readonly property string outputShowTx: groupName + "/outputShowTx"
        readonly property string outputShowDate: groupName + "/outputShowDate"
        readonly property string outputShowTime: groupName + "/outputShowTime"
        readonly property string outputShowMs: groupName + "/outputShowMs"
        readonly property string outputShowWrap: groupName + "/outputShowWrap"
    }

    ColumnLayout {
        anchors.fill: parent
        Repeater {
            model: [
                qsTr("Show read bytes"),
                qsTr("Show written bytes"),
                qsTr("Show date information"),
                qsTr("Show time information"),
                qsTr("Show ms information"),
                qsTr("Show wrap")
            ]
            SAKCheckBox {
                text: modelData
                settingKey: {
                    if (index === 0) {
                        return settingKeys.outputShowRx
                    } else if (index === 1) {
                        return settingKeys.outputShowTx
                    } else if (index === 2) {
                        return settingKeys.outputShowDate
                    } else if (index === 3) {
                        return settingKeys.outputShowTime
                    } else if (index === 4) {
                        return settingKeys.outputShowMs
                    } else if (index === 5) {
                        return settingKeys.outputShowWrap
                    }
                }
                Component.onCompleted: {
                    if (index === 0) {
                        showRx = Qt.binding(function(){return checked})
                    } else if (index === 1) {
                        showTx = Qt.binding(function(){return checked})
                    } else if (index === 2) {
                        showDate = Qt.binding(function(){return checked})
                    } else if (index === 3) {
                        showTime = Qt.binding(function(){return checked})
                    } else if (index === 4) {
                        showMs = Qt.binding(function(){return checked})
                    } else if (index === 5) {
                        showWrap = Qt.binding(function(){return checked})
                    }
                }
            }
        }
    }
} // SAKGroupBox
