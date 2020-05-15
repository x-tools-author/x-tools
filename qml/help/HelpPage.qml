import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root

    property int panelIndex: 0
    property var panelsInfo: [
        ["qrc:/qml/help/HelpPageAboutQt.qml",   qsTr("关于Qt")],
        ["qrc:/qml/help/HelpPageAboutSAK.qml",  qsTr("关于软件")],
        ["qrc:/qml/help/HelpPageBuy.qml",       qsTr("购买软件")],
        ["qrc:/qml/help/HelpPageDonation.qml",  qsTr("支持开发")],
        ["qrc:/qml/help/HelpPageHistory.qml",   qsTr("历史版本")],
        ["qrc:/qml/help/HelpPage3rd.qml",       qsTr("第三方库")]
    ]

    Item {
        id: centralItem
        anchors {
            fill: parent
            margins: 5
        }

        ScrollView {
            id: leftPanelScrollView
            clip: true
            width: 150
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            Column {
                anchors{
                    left: parent.left
                    right: parent.right
                }

                Repeater {
                    model: panelsInfo
                    Button {
                        text: modelData[1]
                        onClicked: {
                            panelIndex = index
                        }
                    }
                }
            }
        }

        Item {
            id: rightPanelScrollView
            clip: true
            width: centralItem.width - leftPanelScrollView.width - 5
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }

            Repeater {
                model: panelsInfo
                Loader {
                    anchors.fill: rightPanelScrollView
                    source: modelData[0]
                    visible: panelIndex === index
                }
            }
        }
    }
}
