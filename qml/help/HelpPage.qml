import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

Item {
    id: root

    property int panelIndex: 0
    property var panelTitles: []
    property var pannelWidth: (rightPanelScrollView.width*0.8)

    ScrollView {
        id: leftPanelScrollView
        clip: true
        width: 150
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        anchors {
            top: parent.top
            bottom: parent.bottom
        }

        background: Rectangle {
            color: "#161616"
        }

        ColumnLayout {
            width: 150
            // 占位
            Item {width: 1; height: 1}
            Repeater {
                id: panelTitlesRepeater
                model: panelTitles
                SAKButton {
                    text: modelData
                    Layout.minimumWidth: (leftPanelScrollView.width-4)
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        panelIndex = index
                    }
                }
            }
        }
    }

    Rectangle {
        id: rightPanelScrollView
        clip: true
        color: "#161616"
        width: root.width - leftPanelScrollView.width - 2
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        ScrollView {
            width: parent.width
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            Column {
                id: pagePanelColumn
                spacing: 10
                width: rightPanelScrollView.width
                topPadding: 5

                HelpPageAboutQt {
                    id: aboutQt
                    title: qsTr("关于Qt")
                    width: pannelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageAboutSAK {
                    id: aboutSAK
                    title: qsTr("关于软件")
                    width: pannelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageFeedback {
                    id: feedback
                    title: qsTr("反馈交流")
                    width: pannelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageHistory {
                    id: history
                    title: qsTr("发布历史")
                    width: pannelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Component.onCompleted: {
        panelTitles.push(aboutQt.title)
        panelTitles.push(aboutSAK.title)
        panelTitles.push(feedback.title)
        panelTitles.push(history.title)
        panelTitlesRepeater.model = panelTitles
    }
}
