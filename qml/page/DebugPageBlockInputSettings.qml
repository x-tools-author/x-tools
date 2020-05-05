import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"
import "qrc:/qml/component"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/设置 (1).png"
    sakTitle: qsTr("输入设置")

    contentItem: Item{
        Grid {
            id: grid
            columns: 2
            anchors.fill: parent
            columnSpacing: 3
            rowSpacing: 8
            leftPadding: 5
            topPadding: 5

            SAKCheckBox {
                text: qsTr("定时发送")
            }

            SAKLineEdit {
                placeholderText: qsTr("单位:ms")
            }

            SAKText {
                text: qsTr("参数模型")
                height: 25
            }

            SAKComboBox {
                model: ["CRC8", "CRC9"]
            }

            SAKText {
                text: qsTr("数据格式")
                height: 25
            }

            SAKComboBox {
                model: ["Bin", "Otc", "Dec", "Otc", "Hex"]
            }
        }
    }
}
