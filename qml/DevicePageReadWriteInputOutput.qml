import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ED.EasyDebug
import "common"

Item {
    id: root

    property EDBaseTool communicationTool: null
    property EDAnalyzerTool analyzer: null

    property int outputFormat: outputTextFormatComboBox.currentValue
    property int inputFormat: inputTextFormatComboBox.currentValue
    property string inputText: inputComboBox.editText
    property var textVar: outputTextArea.textDocument

    property bool showWrap: true
    property bool showDate: true
    property bool showTime: true
    property bool showMs: true
    property bool showTx: true
    property bool showRx: true

    readonly property string rxColor: "red"
    readonly property string txColor: "blue"

    signal invokeOpenDrawer(var pageIndex)
    signal invokeSend()

    onInputFormatChanged: inputComboBox.editText = ""

    QtObject {
        id: keysObj
        readonly property string itemGroup: groupName + "History"
        readonly property string itemArray: "item"
        readonly property string itemAll: "itemAll"
        readonly property string itemText: "text"
        readonly property string itemFormat: "format"
        readonly property string itemCurrentIndex: groupName + "inputCurrentIndex"
    }

    ButtonGroup {
        id: txTxButtonGroup
    }

    Item {
        id: outputPane
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: inputPane.top
        anchors.margins: 4
        ColumnLayout {
            spacing: 0
            anchors.fill: parent
            RowLayout {
                Repeater {
                    model: [
                        [qsTr("Rx"), groupName + "/outputRx"],
                        [qsTr("Tx"), groupName + "/outputTx"],
                        [qsTr("Date"), groupName + "/outputShowDate"],
                        [qsTr("Time"), groupName + "/outputShowTime"],
                        [qsTr("MS"), groupName + "/outputShowMS"],
                        [qsTr("Wrap"), groupName + "/outputShowWrap"]
                    ]
                    EDCheckBox {
                        id: cbCheckBox
                        text: modelData[0]
                        checked: true
                        settingKey: modelData[1]
                        visible: index !== 5
                        Component.onCompleted: {
                            if (index === 0) {
                                showRx = Qt.binding(function (){return checked})
                            } else if (index === 1) {
                                showTx = Qt.binding(function (){return checked})
                            }  else if (index === 2) {
                                showDate = Qt.binding(function (){return checked})
                            } else if (index === 3) {
                                showTime = Qt.binding(function (){return checked})
                            } else if (index === 4) {
                                showMs = Qt.binding(function (){return checked})
                            } else if (index === 5) {
                                showWrap = Qt.binding(function (){return checked})
                            }

                            if (index === 0 || index === 1) {
                                checked = true
                            }
                        }
                    }
                }
                Item { Layout.fillWidth: true }
                EDTextFormatComboBox {
                    id: outputTextFormatComboBox
                    tips: qsTr("Output text format")
                    settingKey: groupName + "outputTextFromat"
                }
                EDButton {
                    icon.source: "qrc:/res/icon/IconClear.svg"
                    onClicked: outputTextArea.clear()
                }
                EDButton {
                    icon.source: "qrc:/res/icon/IconSettings.svg"
                    onClicked: root.invokeOpenDrawer(1)
                }
            }
            EDPane {
                padding: 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                ScrollView {
                    padding: 0
                    anchors.fill: parent
                    EDTextArea {
                        padding: 0
                        id: outputTextArea
                        width: parent.width
                        background: Rectangle {
                            color: Material.background
                        }
                        selectByMouse: true
                        textFormat: Text.RichText
                        wrapMode: showWrap ? Text.WrapAnywhere : Text.NoWrap
                        Component.onCompleted: {
                            var textDoc = outputTextArea.textDocument
                            sakInterface.setMaximumBlockCount(textDoc, 100)
                        }
                    }
                }
            }
        }
    }
    EDHorizontalLine {
        anchors.bottom: inputPane.top
    }
    Item {
        id: inputPane
        height: inputPaneColumnLayout.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 4
        ColumnLayout {
            id: inputPaneColumnLayout
            spacing: 0
            anchors.left: parent.left
            anchors.right: parent.right
            RowLayout {
                Layout.fillWidth: true
                EDComboBox {
                    id: intervalComboBox
                    enabled: communicationTool ? communicationTool.isWorking : null
                    tips: qsTr("Cycle sending interval")
                    model: [
                        qsTr("Disable"),
                        20, 40, 60, 80, 100,
                        200, 400, 600, 800, 1000,
                        2000, 4000, 6000, 8000, 10000
                    ]
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
                EDTextFormatComboBox {
                    id: inputTextFormatComboBox
                    tips: qsTr("Input text format")
                    settingKey: groupName + "/inputTextFormat"
                }
                EDComboBox {
                    id: inputComboBox
                    Layout.fillWidth: true
                    textRole: keysObj.itemText
                    valueRole: keysObj.itemFormat
                    editable: true
                    validator: inputTextFormatComboBox.currentValidator
                    useValueRole: false
                    model: ListModel {
                        id: hisrotyListModel
                    }
                    onActivated: {
                        sakSettings.edSetValue(keysObj.itemCurrentIndex, currentIndex)
                        var ret = inputTextFormatComboBox.indexOfValue(currentValue)
                        if (ret >= 0) {
                            inputTextFormatComboBox.currentIndex = ret
                            editText = currentText
                        }
                    }

                    Component.onCompleted: {
                        var values = sakSettings.edArrayValues(keysObj.itemGroup, keysObj.itemArray, keysObj.itemAll)
                        var item = {}
                        for (var i = 0; i < values.length; i++) {
                            var str = sakInterface.hexString2String(values[i])
                            var jsonObj = JSON.parse(str)
                            item[keysObj.itemText] = jsonObj[keysObj.itemText]
                            item[keysObj.itemFormat] = jsonObj[keysObj.itemFormat]
                            hisrotyListModel.append(item)
                        }

                        var index = sakSettings.edValue(keysObj.itemCurrentIndex)
                        if (index >= 0 && index < hisrotyListModel.count) {
                            currentIndex = index
                            editText = currentText
                            // Todo: no effect, need to fix.
                        }
                    }
                }
                EDButton {
                    icon.source: "qrc:/res/icon/IconList.svg"
                    onClicked: menu.resetMenu()
                    Menu {
                        id: menu
                        Repeater {
                            id: menuRepeater
                            MenuItem { text: modelData; onTriggered: presotrerTool.send(index) }
                        }

                        function resetMenu() {
                            menuRepeater.model = presotrerTool.descriptions
                            menu.open()
                        }
                    }
                }
                EDButton {
                    icon.source: "qrc:/res/icon/IconSettings.svg"
                    onClicked: root.invokeOpenDrawer(2)
                }
                EDButton {
                    id: sendButton
                    text: qsTr("Send")
                    onClicked: root.invokeSend()
                }
            }
        }
    }

    QtObject {
        id: uselessObject
        signal bytesInputted(var flag, var bytes)
        signal bytesOutputted(var flag, var bytes)
    }

    Timer {
        id: cycleSendingTimer
        running: false
        repeat: true
        onTriggered: root.invokeSend()
    }

    Connections {
        target: communicationTool

        function onBytesInputted(flag, inputedBytes) {
            if (showTx) {
                var str = cookedBytes(flag, inputedBytes, false)
                outputTextArea.append(str)
            }
        }
    }

    Connections {
        target: analyzer

        function onBytesOutputted(flag, outputBytes) {
            if (showRx) {
                var str = cookedBytes(flag, outputBytes, true)
                outputTextArea.append(str)
            }
        }
    }

    function appendHisroty (text, format) {
        var count = hisrotyListModel.count
        for (var i = 0; i < count; i++) {
            if (i == 10) {
                return
            }

            var item = hisrotyListModel.get(i)
            if (item[keysObj.itemText] === text) {
                return
            }
        }        

        item = {}
        item[keysObj.itemText] = text
        item[keysObj.itemFormat] = format
        hisrotyListModel.append(item)
        inputComboBox.editText = text

        sakSettings.edRemove(keysObj.itemGroup, keysObj.itemArray)
        var values = []
        count = hisrotyListModel.count
        for (i = 0; i < count; i++) {
            item = hisrotyListModel.get(i)
            var str = JSON.stringify(item)
            var strHex = sakInterface.string2hexString(str)
            values.push(sakInterface.string2hexString(str))
        }
        sakSettings.edSetArrayValues(keysObj.itemGroup, keysObj.itemArray, keysObj.itemAll, values)
    }

    function clearOutput() {
        outputTextArea.clear()
    }

    function cookedBytes(flag, bytes, isRx) {
        var dateTimeInfo = ""
        if (showDate && showTime) {
            if (showMs) {
                dateTimeInfo = sakInterface.dateTimeString("yyyy-MM-dd hh:mm:ss.zzz ");
            } else {
                dateTimeInfo = sakInterface.dateTimeString("yyyy-MM-dd hh:mm:ss ");
            }
        } else if (showDate) {
            dateTimeInfo = sakInterface.dateTimeString("yyyy-MM-dd ");
        } else if (showTime) {
            if (showMs) {
                dateTimeInfo = sakInterface.dateTimeString("hh:mm:ss.zzz ");
            } else {
                dateTimeInfo = sakInterface.dateTimeString("hh:mm:ss ");
            }
        }
        dateTimeInfo = String("<font color=%1>%2</font>").arg(Material.color(Material.Grey)).arg(dateTimeInfo)

        var flagInfo = isRx ? "Rx" : "Tx"
        flagInfo += "(" + flag + ")"
        flagInfo = isRx ? String("<font color=%1>%2</font>").arg(Material.color(Material.Red)).arg(flagInfo)
                        : String("<font color=%1>%2</font>").arg(Material.color(Material.Blue)).arg(flagInfo)

        var cookedString = "[" + dateTimeInfo + " " + flagInfo + "] "
        cookedString =  String("<font color=silver>%1</font>").arg(cookedString)
        var bytesString = sakInterface.array2String(bytes, outputFormat)
        cookedString += bytesString

        return cookedString
    }
}
