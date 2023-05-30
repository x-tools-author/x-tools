import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import SAK.Custom
import "common"

Item {
    id: root
    Layout.minimumWidth: 250
    Layout.fillHeight: true
    Layout.rowSpan: 2

    property EDBaseTool communicationTool: null
    property var controllerComponent: null
    property alias deviceControllerLoader: controllerLoader
    property int outputFormat: outputFormatComboBox.currentValue
    property int inputFormat: inputFormatComboBox.currentValue
    property bool devIsWorking: communicationTool ? communicationTool.isWorking : false

    signal invokeOpenDrawer(var pageIndex)
    signal invokeOpenDevice()
    signal invokeCloseDevice()
    signal invokeSend();
    signal invokeClearOutput();

    onDevIsWorkingChanged: {
        if (!devIsWorking) {
            intervalComboBox.currentIndex = 0
            cycleSendingTimer.stop()
        }
    }

//    background: Rectangle {
//        radius: 8
//        layer.enabled: true
//        layer.effect: DropShadow {
//            samples: 16
//            radius: 8
//        }
//    }

    QtObject {
        id: settingKeys
        readonly property string outputFormat: groupName + "/outputFormat"
        readonly property string inputFormat: groupName + "/inputFormat"
    }

    Timer {
        id: cycleSendingTimer
        running: false
        repeat: true
        onTriggered: root.invokeSend()
    }

    ListModel {
        id: inputTextFormatListModel
        ListElement { text: "Hex"; value: EDDataStructure.TextFormatHex }
        ListElement { text: "Ascii"; value: EDDataStructure.TextFormatAscii }
        ListElement { text: "Utf8"; value: EDDataStructure.TextFormatUtf8 }
    }

    GridLayout {
        id: contentGridLayout
        anchors.fill: parent
        columns: 2
        anchors.rightMargin: 6
        anchors.leftMargin: 6
        Item {Layout.minimumHeight: 2; Layout.columnSpan: 2}
        Loader {
            id: controllerLoader
            Layout.fillWidth: true
            sourceComponent: controllerComponent
            Layout.columnSpan: 2
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            EDButton {
                text: qsTr("Conf")
                Layout.fillWidth: true
                onClicked: root.invokeOpenDrawer(0)
            }
            EDButton {
                text: {
                    if (communicationTool) {
                        return communicationTool.isWorking
                                ? qsTr("Close")
                                : qsTr("Open")
                    }
                }
                Layout.fillWidth: true
                onClicked: {
                    if (communicationTool) {
                        if (communicationTool.isWorking) {
                            invokeCloseDevice()
                        } else {
                            invokeOpenDevice()
                        }
                    }
                }
            }
        }

        EDGroupBox {
            title: qsTr("Output settings")
            Layout.fillWidth: true
            visible: false
            GridLayout {
                anchors.fill: parent
                columns: 2
                EDLabel {
                    text: qsTr("Outut format")
                }
                EDTextFormatComboBox {
                    id: outputFormatComboBox
                    Layout.fillWidth: true
                    settingKey: settingKeys.outputFormat
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    EDButton {
                        text: qsTr("Conf")
                        Layout.fillWidth: true
                        onClicked: root.invokeOpenDrawer(1)
                    }
                    EDButton {
                        text: qsTr("Clear")
                        Layout.fillWidth: true
                        onClicked: root.invokeClearOutput()
                    }
                }
            }
        }
        Label {
            width: 1
            Layout.columnSpan: 2
            Layout.fillHeight: true
        }
        EDGroupBox {
            title: qsTr("Input settings")
            Layout.fillWidth: true
            visible: false
            GridLayout {
                anchors.fill: parent
                columns: 2
                EDLabel {
                    text: qsTr("Timer sending")
                }
                EDComboBox {
                    id: intervalComboBox
                    enabled: communicationTool ? communicationTool.isWorking : null
                    model: [
                        qsTr("Disable"),
                        20, 40, 60, 80, 100,
                        200, 400, 600, 800, 1000,
                        2000, 4000, 6000, 8000, 10000
                    ]
                    Layout.fillWidth: true

                    onActivated: {
                        if (intervalComboBox.currentIndex === 0) {
                            cycleSendingTimer.stop()
                        } else {
                            var interval = Number(intervalComboBox.currentText)
                            cycleSendingTimer.interval = interval
                            cycleSendingTimer.restart()
                        }
                    }
                }
                EDLabel {
                    text: qsTr("Input format")
                }
                EDComboBox {
                    id: inputFormatComboBox
                    model: inputTextFormatListModel
                    textRole: "text"
                    valueRole: "value"
                    Layout.fillWidth: true
                    settingKey: settingKeys.inputFormat
                }
                Grid {
                    id: btGrid
                    spacing: contentGridLayout.columnSpacing
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    Repeater {
                        model: [qsTr("Conf"), qsTr("Send")]
                        EDButton {
                            text: modelData
                            //width: (parent.width - contentGridLayout.columnSpacing)/2
                            enabled: {
                                if (index === 0) {
                                    return true
                                } else  {
                                    return communicationTool ? communicationTool.isWorking : false
                                }
                            }
                            onClicked: {
                                if (index === 0) {
                                    root.invokeOpenDrawer(2)
                                } else {
                                    root.invokeSend()
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {Layout.minimumHeight: 4}
    }
}
