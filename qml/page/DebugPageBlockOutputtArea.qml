import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/输出.png"
    sakTitle: qsTr("数据输出")

    contentItem: Item{
        SAKTextArea {
            readOnly: true
            anchors {
                fill: parent
                margins: 5
            }
        }
    }
}
