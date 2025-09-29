import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import xTools.xDebug

ColumnLayout {
    id: root
    spacing: 0
    required property string settingsGroup

    Timer {
        id: txTimer
        repeat: true
        running: false
        onTriggered: writeBytes()
    }

    QtObject {
        id: settingKeys
        readonly property string format: settingsGroup + "/" + "format"
        readonly property string esc: settingsGroup + "/" + "esc"
        readonly property string prefix: settingsGroup + "/" + "prefix"
        readonly property string suffix: settingsGroup + "/" + "suffix"
        readonly property string crc: settingsGroup + "/" + "crc"
    }

    RowLayout {
        spacing: 0
        Layout.fillWidth: true
        BasePageIconItem {
            id: timerToolItem
            valueName: xDisabledText
            value: -1
            iconSource: "qrc:/res/icons/timer.svg"
            isActionGroup: true
            enabled: isWorking
            toolTipText: qsTr("Timed data")
            Layout.alignment: Qt.AlignVCenter
            onEnabledChanged: {
                if (enabled && Number(value) !== -1) {
                    txTimer.start();
                } else {
                    txTimer.stop();
                }
            }
            onValueChanged: {
                if (value == -1) {
                    txTimer.stop();
                } else {
                    if (isWorking) {
                        txTimer.interval = value;
                        txTimer.restart();
                    }
                }
            }
            menuItemContext: {
                var tmp = [];
                tmp.push([xDisabledText, -1]);
                for (var i = 20; i <= 100; i += 20) {
                    var item = [];
                    item.push(i.toString() + "ms");
                    item.push(i);
                    tmp.push(item);
                }
                for (i = 200; i <= 1000; i += 100) {
                    item = [];
                    item.push(i.toString() + "ms");
                    item.push(i);
                    tmp.push(item);
                }
                for (i = 2000; i <= 10000; i += 2000) {
                    item = [];
                    item.push(i.toString() + "ms");
                    item.push(i);
                    tmp.push(item);
                }

                return tmp;
            }
        }
        BasePageIconItem {
            id: formatToolItem
            valueName: "Hex"
            iconSource: "qrc:/res/icons/format.svg"
            isActionGroup: true
            menuItemContext: formatMenuContext()
            toolTipText: qsTr("Set the text format of input text")
            Layout.alignment: Qt.AlignVCenter
            onHasBeenTriggered: xSettings.setValue(settingKeys.format, value)
        }
        BasePageIconItem {
            id: escToolItem
            valueName: "None"
            iconSource: "qrc:/res/icons/esc.svg"
            isActionGroup: true
            toolTipText: qsTr("Escape character")
            Layout.alignment: Qt.AlignVCenter
            onHasBeenTriggered: xSettings.setValue(settingKeys.esc, value)
            menuItemContext: {
                var tmp = [];
                tmp.push([xNoneText, xEscNone]);
                tmp.push(["\\n", xEscN]);
                tmp.push(["\\r", xEscR]);
                tmp.push(["\\r\\n", xEscRN]);
                tmp.push(["\\n\\r", xEscNR]);
                tmp.push(["\\r || \\n", xEscR_N]);
                return tmp;
            }
        }
        BasePageIconItem {
            id: prefixToolItem
            valueName: "None"
            iconSource: "qrc:/res/icons/header.svg"
            isActionGroup: true
            menuItemContext: affixesMenuContext()
            toolTipText: qsTr("The prefix data")
            Layout.alignment: Qt.AlignVCenter
            onHasBeenTriggered: xSettings.setValue(settingKeys.prefix, value)
        }
        BasePageIconItem {
            id: suffixToolItem
            valueName: "None"
            iconSource: "qrc:/res/icons/tail.svg"
            toolTipText: qsTr("The suffix data")
            isActionGroup: true
            menuItemContext: affixesMenuContext()
            Layout.alignment: Qt.AlignVCenter
            onHasBeenTriggered: xSettings.setValue(settingKeys.suffix, value)
        }
        BasePageIconItem {
            id: crcToolItem
            valueName: "None"
            iconSource: "qrc:/res/icons/check.svg"
            toolTipText: qsTr("The CRC algorithm")
            isActionGroup: true
            Layout.alignment: Qt.AlignVCenter
            onHasBeenTriggered: xSettings.setValue(settingKeys.crc, value)
            menuItemContext: {
                var tmp = [];
                tmp.push([xNoneText, -1]);
                tmp.push(["CRC_8", CRC.Algorithm.CRC_8]);
                tmp.push(["CRC_8_ITU", CRC.Algorithm.CRC_8_ITU]);
                tmp.push(["CRC_8_ROHC", CRC.Algorithm.CRC_8_ROHC]);
                tmp.push(["CRC_8_MAXIM", CRC.Algorithm.CRC_8_MAXIM]);

                tmp.push(["CRC_16_IBM", CRC.Algorithm.CRC_16_IBM]);
                tmp.push(["CRC_16_MAXIM", CRC.Algorithm.CRC_16_MAXIM]);
                tmp.push(["CRC_16_USB", CRC.Algorithm.CRC_16_USB]);
                tmp.push(["CRC_16_MODBUS", CRC.Algorithm.CRC_16_MODBUS]);
                tmp.push(["CRC_16_CCITT", CRC.Algorithm.CRC_16_CCITT]);
                tmp.push(["CRC_16_CCITT_FALSE", CRC.Algorithm.CRC_16_CCITT_FALSE]);
                tmp.push(["CRC_16_x25", CRC.Algorithm.CRC_16_x25]);
                tmp.push(["CRC_16_XMODEM", CRC.Algorithm.CRC_16_XMODEM]);
                tmp.push(["CRC_16_DNP", CRC.Algorithm.CRC_16_DNP]);

                tmp.push(["CRC_32", CRC.Algorithm.CRC_32]);
                tmp.push(["CRC_32_MPEG2", CRC.Algorithm.CRC_32_MPEG2]);
                return tmp;
            }
        }
        Label {
            Layout.fillWidth: true
        }
        ControlsToolButton {
            icon.source: "qrc:/res/icons/clear.svg"
            onClicked: inputTextComboBox.clear()
            Layout.alignment: Qt.AlignVCenter
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Clear the input text")
        }
    }
    RowLayout {
        Layout.fillWidth: true
        TextField {
            id: inputTextComboBox
            onValidatorTypeChanged: clear()
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.minimumHeight: 34
            Layout.maximumHeight: 34
            placeholderText: qsTr("[prefix][input][crc][suffix](crc data is input data)")
            validator: {
                switch (validatorType) {
                case xBin:
                    return binREV;
                case xOct:
                    return octREV;
                case xDec:
                    return decREV;
                case xHex:
                    return hexREV;
                case xAscii:
                    return asciiREV;
                default:
                    return null;
                }
            }
            property string settingKey: ""
            property int validatorType: formatToolItem.value

            RegularExpressionValidator {
                id: binREV
                regularExpression: /([01][01][01][01][01][01][01][01][ ])*/
            }
            RegularExpressionValidator {
                id: octREV
                regularExpression: /^(0[0-7]{0,2}|[1-3][0-7]{2})( (0[0-7]{0,2}|[1-3][0-7]{2}))*/
            }
            RegularExpressionValidator {
                id: decREV
                regularExpression: /^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9])( (25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]))*/
            }
            RegularExpressionValidator {
                id: hexREV
                regularExpression: /([0-9a-fA-F][0-9a-fA-F][ ])*/
            }
            RegularExpressionValidator {
                id: asciiREV
                regularExpression: /([ -~])*/
            }
        }
        ControlsButton {
            icon.source: "qrc:/res/icons/send.svg"
            text: qsTr("Send")
            enabled: isWorking
            onClicked: writeBytes()
            Layout.minimumHeight: inputTextComboBox.height
            Layout.rightMargin: 4
        }
    }
    Label {
        text: ""
        Layout.maximumHeight: 4
    }

    Component.onCompleted: {
        xApp.showSplashScreenMessage("Loading input settings for: " + settingsGroup);

        var format = xSettings.value(settingKeys.format, xHex);
        var esc = xSettings.value(settingKeys.esc, xEscNone);
        var prefix = xSettings.value(settingKeys.prefix, xAffixesNone);
        var suffix = xSettings.value(settingKeys.suffix, xAffixesNone);
        var crc = xSettings.value(settingKeys.crc, -1);

        formatToolItem.setValue(format);
        escToolItem.setValue(esc);
        prefixToolItem.setValue(prefix);
        suffixToolItem.setValue(suffix);
        crcToolItem.setValue(crc);
    }

    function writeBytes() {
        var format = formatToolItem.value;
        var esc = escToolItem.value;
        var header = prefixToolItem.value;
        var tail = suffixToolItem.value;
        var crc = crcToolItem.value;

        var txt = inputTextComboBox.text;
        if (txt === "") {
            txt = "(null)";
            format = xAscii;
        }

        var inputString = xApp.cookedEscapeCharacter(txt, esc);
        var prefixStr = xApp.cookedAffixes(header);
        var suffixStr = xApp.cookedAffixes(tail);

        var prefixBytes = xApp.string2bytes(prefixStr, format);
        var inputBytes = xApp.string2bytes(inputString, format);
        var crcBytes = xCrc.calculate(inputBytes, crc);
        var suffixBytes = xApp.string2bytes(suffixStr, format);

        var cookedBytes = xApp.arrayAppendArray(prefixBytes, inputBytes);
        cookedBytes = xApp.arrayAppendArray(cookedBytes, crcBytes);
        cookedBytes = xApp.arrayAppendArray(cookedBytes, suffixBytes);
        if (device) {
            device.writeBytes(cookedBytes);
        }
    }
}
