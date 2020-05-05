import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/设置 (1).png"
    sakTitle: qsTr("输出设置")

    property var _leftPadding: 5

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
                model: ["Bin", "Otc", "Dec", "Hex", "Ascii", "Utf8", "System"]
            }
            Repeater {
                model: [qsTr("输出日期"), qsTr("输出时间"), qsTr("显示毫秒"), qsTr("自动换行"), qsTr("显示接收"), qsTr("显示发送")]

                SAKCheckBox {
                    text: modelData
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
                    qsTr("发送速率"), "0",
                    qsTr("发送帧数"), "0",
                    qsTr("发送字节"), "0",
                    qsTr("接收速率"), "0",
                    qsTr("接收帧数"), "0",
                    qsTr("接收字节"), "0"
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
                        console.info(modelData)
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
                model: [qsTr("清空输入"), qsTr("保存输入")]

                SAKButton {
                    text: modelData
                    height: 25

                    onClicked: {
                        console.info(modelData)
                    }
                }
            }
        }
    }
}
