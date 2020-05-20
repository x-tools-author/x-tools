import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

Item {
    id: root

    property int panelIndex: 0
    property var panelTitles: []
    property var panelWidth: (rightPanelRect.width*0.8)
    property var panelHeight: 0
    property var panelItemsHeight: []

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
                        var h = 0
                        for (var i = 0; i < index; i++){
                            h = h + panelItemsHeight[i]
                        }
                        verticalScrollBar.position = h/panelHeight
                    }
                }
            }
        }
    }

    Rectangle {
        id: rightPanelRect
        clip: true
        color: "#161616"
        width: root.width - leftPanelScrollView.width - 2
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }



        ScrollView {
            id: rightPanelScrollView
            width: parent.width
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical:  ScrollBar {
                id: verticalScrollBar
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                hoverEnabled: true
            }

            Column {
                id: pagePanelColumn
                spacing: 10
                width: rightPanelRect.width
                topPadding: 5

                HelpPageAboutQt {
                    id: aboutQt
                    title: qsTr("关于Qt")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageAboutSAK {
                    id: aboutSAK
                    title: qsTr("关于软件")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPage3rd {
                    id: panel3rd
                    title: qsTr("第三方库")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageFeedback {
                    id: feedback
                    title: qsTr("反馈交流")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageBuy {
                    id: pageBuy
                    title: qsTr("软件购买")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                HelpPageHistory {
                    id: history
                    title: qsTr("发布历史")
                    width: panelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Component.onCompleted: {
        panelTitles.push(aboutQt.title)
        panelTitles.push(aboutSAK.title)
        panelTitles.push(panel3rd.title)
        panelTitles.push(feedback.title)
        panelTitles.push(pageBuy.title)
         panelTitles.push(history.title)
        panelTitlesRepeater.model = panelTitles

        panelItemsHeight.push(aboutQt.height    + pagePanelColumn.topPadding)
        panelItemsHeight.push(aboutSAK.height   + 5)
        panelItemsHeight.push(panel3rd.height   + 5)
        panelItemsHeight.push(feedback.height   + 5)
        panelItemsHeight.push(pageBuy.height    + 5)
        panelItemsHeight.push(history.height    + 5 + pagePanelColumn.bottomPadding)

        for (var i = 0; i < panelItemsHeight.length; i++){
            panelHeight += panelItemsHeight[i]
        }
    }
}
