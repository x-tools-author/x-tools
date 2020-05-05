import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/其他.png"
    sakTitle: qsTr("其他设置")

    contentItem: Item{
        Grid {
            id: grid
            columns: 2
            anchors.fill: parent
            columnSpacing: 15
            rowSpacing: 8
            leftPadding: 5
            topPadding: 5

            Repeater {
                model: [
                    qsTr("数据转发"),
                    qsTr("读写参数"),
                    qsTr("定时发送"),
                    qsTr("自动回复"),
                    qsTr("高亮设置"),
                    qsTr("更多设置")
                ]
                SAKButton {
                    height: 25
                    text: modelData

                    onClicked: {
                        console.info(modelData)
                    }
                }
            }
        }
    }
}
