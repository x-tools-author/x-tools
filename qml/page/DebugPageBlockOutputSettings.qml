import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/设置 (1).png"
    sakTitle: qsTr("输出设置")

    property SAKDebuggerOutputManager outputManager: sakdebugger ? sakdebugger.outputManager : null
    property SAKDebuggerOutputSettings outputSettings: outputManager ? outputManager.outputSettings : null
    property SAKDebuggerOutputStatistics statistics: outputSettings ? outputSettings.statistics : null
    property var _leftPadding: 5

    signal outputSettingsHasChanged();

    onOutputSettingsChanged: {
        outputSettingsHasChanged()
    }

    contentItem: Item{
        Grid {
            id: grid
            columns: 2
            anchors.fill: parent
            columnSpacing: 1
            rowSpacing: 0
            leftPadding: _leftPadding
            topPadding: 5

            SAKLabel {
                text: qsTr("数据格式")
                height: 25
                verticalAlignment: Text.AlignVCenter
            }
            SAKComboBox {
                model: outputSettings ? outputSettings.textFormats : 9
                onModelChanged: {
                    if (outputSettings){
                        var ret = find(outputSettings.currentTextFormat)
                        if (ret > 0){
                            currentIndex = ret
                        }
                    }
                }

                onCurrentTextChanged: {
                    if (outputSettings){
                        outputSettings.currentTextFormat = currentText
                    }
                }
            }
            Repeater {
                model: [
                    [qsTr("输出日期"), false],
                    [qsTr("输出时间"), true],
                    [qsTr("显示毫秒"), false],
                    [qsTr("自动换行"), false],
                    [qsTr("显示接收"), true],
                    [qsTr("显示发送"), true]
                ]

                SAKCheckBox {
                    id: checkBox
                    text: modelData[0]
                    onCheckedChanged: {
                        if (outputSettings){
                            switch (index){
                            case 0:
                                outputSettings.outputDate = checked
                                break;
                            case 1:
                                outputSettings.outputTime = checked
                                break;
                            case 2:
                                outputSettings.outputMs = checked
                                break;
                            case 3:
                                outputSettings.outputWrap = checked
                                break;
                            case 4:
                                outputSettings.outputRx = checked
                                break;
                            case 5:
                                outputSettings.outputTx = checked
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    Component.onCompleted: {
                        checked = modelData[1]
                    }

                    Connections {
                        target: root
                        onOutputSettingsHasChanged: {
                            if (outputSettings){
                                switch (index){
                                case 0:
                                   outputSettings.outputDate = checkBox.checked
                                    break;
                                case 1:
                                    outputSettings.outputTime  = checkBox.checked
                                    break;
                                case 2:
                                    outputSettings.outputMs = checkBox.checked
                                    break;
                                case 3:
                                    outputSettings.outputWrap = checkBox.checked
                                    break;
                                case 4:
                                    outputSettings.outputRx = checkBox.checked
                                    break;
                                case 5:
                                    outputSettings.outputTx = checkBox.checked
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // 占位
            Repeater {
                model: 2
                Item {
                    width: 60
                    height: 5
                }
            }

            Repeater {
                model: [
                    qsTr("发送速率"), statistics ? statistics.txSpeed  : "0B/s",
                    qsTr("发送帧数"), statistics ? statistics.txFrames : "0",
                    qsTr("发送字节"), statistics ? statistics.txBytes  : "0",
                    qsTr("接收速率"), statistics ? statistics.rxSpeed  : "0B/s",
                    qsTr("接收帧数"), statistics ? statistics.rxFrames : "0",
                    qsTr("接收字节"), statistics ? statistics.rxBytes  : "0",
                ]
                SAKLabel {
                    id: sakLabel
                    text: modelData
                    height: 25
                    property var number: index
                    Component.onCompleted: {
                        if (sakLabel.number%2 === 1){
                            sakLabel.font.pixelSize = 14
                            if (sakLabel.number < 6){
                                sakLabel.color = "green"
                            }else{
                                sakLabel.color = "purple"
                            }
                        }
                    }
                }
            }
            Repeater {
                model: [
                    [qsTr("复位接收"), "green"],
                    [qsTr("复位发送"), "purple"]
                ]

                SAKButton {
                    text: modelData[0]
                    height: 25
                    color: modelData[1]

                    onClicked: {
                        if (statistics && (index == 0)){
                            statistics.clearRxStatistics()
                        }

                        if (statistics && (index == 1)){
                            statistics.clearTxStatistics()
                        }
                    }
                }
            }
            // 占位
            Repeater {
                model: 2
                Item {
                    width: 60
                    height: 8
                }
            }
            Repeater {
                model: [qsTr("清空输出"), qsTr("保存输出")]

                SAKButton {
                    text: modelData
                    height: 25

                    onClicked: {
                        if (outputSettings){
                            if (index === 0){
                                outputSettings.clearOutput()
                            }

                            if (index == 1){
                                outputSettings.saveOutputData()
                            }
                        }
                    }
                }
            }
        }
    }
}
