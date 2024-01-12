import QtQuick
import QtQuick.Layouts

import "../common"

SAKPane {
    id: root
    contentItem: Item {
        GridLayout {
            columns: 2
            SAKLabel {
                text: qsTr("Version:")
            }
            SAKLabel {
                text: Application.version
            }
            SAKLabel {
                text: qsTr("Report a bug:")
            }
            SAKLabel {
                text: "qsaker@foxmail.com"
            }
            SAKLabel {
                text: qsTr("Build info:")
            }
            SAKLabel {
                text: sakInterface.buildDateTime(sakInterface.dateFormat(
                                                     ) + " " + sakInterface.timeFormat())
            }
            SAKLabel {
                text: qsTr("User QQ Group:")
            }
            Row {
                spacing: 8
                SAKLabel {
                    id: qqGroupLabel
                    text: "952218522"
                    anchors.verticalCenter: parent.verticalCenter
                }
                SAKButton {
                    text: qsTr("Copy")
                    onClicked: sakInterface.setClipboardText(qqGroupLabel.text)
                }
            }
            SAKLabel {
                text: qsTr("Copyright information:")
            }
            SAKLabel {
                text: String("Copyright 2023-%1 Qsaker. All rights reserved.").arg(
                          sakInterface.buildDateTime("yyyy"))
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
