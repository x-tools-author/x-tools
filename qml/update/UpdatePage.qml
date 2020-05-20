import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"
import SAK.CustomType 1.0

Item {
    id: root
    property var updateManager: SAKUpdateManager

    Rectangle {
        anchors.fill: parent
        color: "#161616"
    }

    GridLayout {
        id: gridLayout
        columns: 5
        columnSpacing: 10
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 5
        }

        SAKLabel {text: qsTr("当前版本")}
        SAKLabel {text: updateManager.appVersion; color: "red"}
        SAKLabel {text: qsTr("更新版本")}
        SAKLabel {text: updateManager.remoteVersion; color: "red"}
        SAKLabel {text: " "; Layout.fillWidth: true}
    }
}
