import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

HelpPagePanel {
    id: root

    property SAKHelpAboutQtController aboutQtController: helpManager.aboutQtController

    property var infos: [
        qsTr("Qt版本"), aboutQtController.qtVersion,
        qsTr("Qt官网"), aboutQtController.officialWeb,
        qsTr("官方发布地址"), aboutQtController.officialRelease,
        qsTr("中国镜像地址"), aboutQtController.chineseRelease,
        qsTr("官方博客"), aboutQtController.officialBlog,
        qsTr("发布维基"), aboutQtController.officialReleaseWiki
    ]

    GridLayout {
        width: root.width
        columns: 2

        Repeater {
            model: infos
            SAKLabel {
                text: modelData
                color: mouseArea.containsMouse ? "#0000ff" : "#ffffff"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    visible: index%2

                    onClicked: {
                        if (index != 1){
                            aboutQtController.openUrl(modelData)
                        }
                    }
                }
            }
        }
    }
}
