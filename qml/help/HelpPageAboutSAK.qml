import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

HelpPagePanel {
    id: root
    property SAKHelpAboutSAKController aboutSAKController: helpManager.aboutSAKController

    property var infos: [
        qsTr("软件版本"), aboutSAKController.appVersion,
        qsTr("构建信息"), aboutSAKController.buildDateTime,
        qsTr("软件作者"), aboutSAKController.authorName,
        qsTr("联系邮箱"), aboutSAKController.authorEmail,
        qsTr("版权声明"), aboutSAKController.copyright,
    ]

    property var urls: [
        qsTr("github托管站点"), aboutSAKController.githubUrl,
        qsTr("gittee托管站点"), aboutSAKController.gitteeUrl,
        qsTr("qsak发布站点"), aboutSAKController.officialRelease
    ]

    GridLayout {
        width: root.width
        columns: 2

        Repeater {
            model: infos
            SAKLabel {
                text: modelData
            }
        }

        Repeater {
            model: urls
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
                        aboutSAKController.openUrl(modelData)
                    }
                }
            }
        }
    }
}
