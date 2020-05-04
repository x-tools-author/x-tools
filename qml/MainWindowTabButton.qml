import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/page"

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("瑞士军刀")
    flags: Qt.FramelessWindowHint | Qt.Dialog

    Page {
        anchors.fill: parent
        background: Rectangle{color: "#222222"}
        header: MainWindowToolBar{}
        contentItem: Item{
            MainWindowTabBar {
                id: tabBar
                anchors{left: parent.left;right: parent.right;top:parent.top}
            }
        }
    }
}
