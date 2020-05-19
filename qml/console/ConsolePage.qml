import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

Item {
    id: root
    anchors.fill: parent

//    property var consoleManager: SAKConsoleManager

//    Rectangle {
//        anchors.fill: parent
//        color: "#161616"
//    }

//    ScrollView {
//        clip: true
//        anchors {
//            fill: parent
//            margins: 5
//        }

//        ColumnLayout {
//            width: parent.width
//            Repeater {
//                model: consoleManager.messagesList
//                SAKLabel {
//                    text: msgItem.msg
//                    color: msgItem.color
//                    wrapMode: Text.WrapAnywhere
//                    Layout.fillWidth: true

//                    property SAKConsoleMessage msgItem: modelData
//                }
//            }
//        }
//    }
}
