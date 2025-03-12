import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import xTools.xDebug

Item {
    id: root

    property int format: formatToolItem.value
    required property string settingsGroup
    property string rxColor: Material.color(Material.Red, xColorShade)
    property string txColor: Material.color(Material.Blue, xColorShade)
    property bool showRx: true
    property bool showTx: true
    property bool showDate: false
    property bool showTime: true
    property bool showMs: false
    property bool autoWrap: false

    QtObject {
        id: settingKeys
        readonly property string format: settingsGroup + "/" + "format"
        readonly property string showRx: settingsGroup + "/" + "showRx"
        readonly property string showTx: settingsGroup + "/" + "showTx"
        readonly property string showDate: settingsGroup + "/" + "showDate"
        readonly property string showTime: settingsGroup + "/" + "showTime"
        readonly property string showMs: settingsGroup + "/" + "showMs"
        readonly property string autoWrap: settingsGroup + "/" + "autoWrap"
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        RowLayout {
            BasePageIconItem {
                id: formatToolItem
                valueName: "Hex"
                iconSource: "qrc:/res/icons/format.svg"
                toolTipText: qsTr("Set the text format of output text")
                isActionGroup: true
                value: xHex
                menuItemContext: formatMenuContext()
                Layout.alignment: Qt.AlignVCenter
                onHasBeenTriggered: xSettings.setValue(settingKeys.format, formatToolItem.value)
            }

            Repeater {
                model: {
                    var tmp = []
                    tmp.push([qsTr("Show read text"), "qrc:/res/icons/rx.svg"])
                    tmp.push([qsTr("Show written data"), "qrc:/res/icons/tx.svg"])
                    tmp.push([qsTr("Show date "), "qrc:/res/icons/date.svg"])
                    tmp.push([qsTr("Show time "), "qrc:/res/icons/time.svg"])
                    tmp.push([qsTr("Show ms "), "qrc:/res/icons/ms.svg"])
                    tmp.push([qsTr("Auto wrap"), "qrc:/res/icons/enter.svg"])
                    return tmp
                }

                ToolButton {
                    id: tb
                    icon.source: modelData[1]
                    checkable: true
                    ToolTip.visible: hovered
                    ToolTip.text: modelData[0]
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: {
                        if (index === 0) {
                            showRx = checked
                            xSettings.setValue(settingKeys.showRx, checked)
                        } else if (index === 1) {
                            showTx = checked
                            xSettings.setValue(settingKeys.showTx, checked)
                        } else if (index === 2) {
                            showDate = checked
                            xSettings.setValue(settingKeys.showDate, checked)
                        } else if (index === 3) {
                            showTime = checked
                            xSettings.setValue(settingKeys.showTime, checked)
                        } else if (index === 4) {
                            showMs = checked
                            xSettings.setValue(settingKeys.showMs, checked)
                        } else if (index === 5) {
                            autoWrap = checked
                            xSettings.setValue(settingKeys.autoWrap, checked)
                        }
                    }
                    Component.onCompleted: {
                        if (index === 0) {
                            // console.info(settingsGroup, "123", ret, typeof ret)
                            // ret是string类型
                            var ret = xSettings.value(settingKeys.showRx, true)
                            checked = true //(ret === xTrue ? true : false)
                            showRx = checked
                        } else if (index === 1) {
                            ret = xSettings.value(settingKeys.showTx, true)
                            checked = true //(ret === xTrue ? true : false)
                            showTx = checked
                        } else if (index === 2) {
                            ret = xSettings.value(settingKeys.showDate, false)
                            checked = (ret === xTrue ? true : false)
                            showDate = checked
                        } else if (index === 3) {
                            ret = xSettings.value(settingKeys.showTime, true)
                            checked = (ret === xTrue ? true : false)
                            showTime = checked
                        } else if (index === 4) {
                            ret = xSettings.value(settingKeys.showMs, false)
                            checked = (ret === xTrue ? true : false)
                            showMs = checked
                        } else if (index === 5) {
                            ret = xSettings.value(settingKeys.autoWrap, false)
                            checked = (ret === xTrue ? true : false)
                            autoWrap = checked
                        }
                    }
                }
            }
            Label {
                height: 1
                Layout.fillWidth: true
            }
            ToolButton {
                id: clearToolButton
                icon.source: "qrc:/res/icons/clear.svg"
                onClicked: outputTextArea.clear()
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Clear the output text")
                Layout.alignment: Qt.AlignVCenter
            }
        }
        ScrollView {
            anchors.margins: 0
            contentWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            TextArea {
                id: outputTextArea
                padding: 0
                selectByMouse: true
                width: parent.width
                textFormat: TextEdit.RichText
                wrapMode: autoWrap ? TextEdit.Wrap : TextEdit.NoWrap
                background: Item {}
                Component.onCompleted: xApp.setQuickTextDocumentMaximumBlockCount(textDocument, 256)
                CommonSeparator {
                    edge: Qt.TopEdge
                }
            }
        }
    }

    Connections {
        target: device
        function onBytesRead(bytes, from) {
            var txt = xApp.bytes2string(bytes, format)
            if (showRx) {
                outputText(txt, from, true)
            }
        }
        function onBytesWritten(bytes, to) {
            var txt = xApp.bytes2string(bytes, format)
            if (showTx) {
                outputText(txt, to, false)
            }
        }
    }

    Component.onCompleted: {
        xApp.showSplashScreenMessage("Loading output settings for: " + settingsGroup)

        var format = xSettings.value(settingKeys.format, xHex)
        formatToolItem.setValue(format ? format : xHex)
    }

    function outputText(txt, fromTo, isRx) {
        var dtFlag = dateTimeFlag()
        var outFlag = outputFlag(isRx, fromTo, dtFlag)
        var cookedTxt = "<font color=%1>%2</font>"
        cookedTxt = cookedTxt.arg(Material.foreground).arg(txt)
        outputTextArea.append(outFlag + cookedTxt)
    }

    function outputFlag(isRx, target, dateTime) {
        var outputColor = isRx ? rxColor : txColor
        var fromTo = isRx ? "Rx" : "Tx"
        //&lt; <
        //&gt; >
        var flag = isRx ? ("%1&lt;-").arg(target) : ("-&gt;%1").arg(target)
        var txt = "<font color=silver>[<font color=%1>%2</font> (%3) %4] </font>"
        txt = txt.arg(outputColor).arg(fromTo).arg(flag).arg(dateTime)
        return txt
    }

    function dateTimeFlag() {
        var str = ""
        if (showDate) {
            str = str + xApp.dateTimeString("yyyy-MM-dd ")
        }
        if (showTime) {
            str = str + xApp.dateTimeString("hh:mm:ss")
            if (showMs) {
                str = str + "." + xApp.dateTimeString("zzz")
            }
        }
        return str
    }
}
