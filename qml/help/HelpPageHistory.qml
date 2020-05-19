import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0
import "qrc:/qml/base"

HelpPagePanel {
    id: root
    implicitHeight: 350

    property SAKHelpHistoryController historuController: helpManager.historyController

    ScrollView {
        width: (root.width - root.leftPadding - root.rightPadding)
        height: 300
        clip: true

        SAKTextArea {
            id: historyTextArea
            implicitWidth:  (root.width - root.leftPadding - root.rightPadding)
            implicitHeight: 300
        }
    }

    Component.onCompleted: historyTextArea.text = historuController.readinHistoryInfo()
}
