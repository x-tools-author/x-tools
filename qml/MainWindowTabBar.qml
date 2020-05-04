import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    id: root
    color: "#101010"
    height: 28

    property var tabButtons: [qsTr("串口调试"), qsTr("UDP调试"), qsTr("TCP客户端"), qsTr("TCP服务器")]
    property int currentIndex: 0

    RowLayout {
        spacing: 10
        anchors{left: parent.left;top: parent.top;bottom: parent.bottom;leftMargin: 10}

        Repeater {
            id: tabButtonsRepeater
            model: tabButtons

            Label {
                height: parent.height
                text: modelData
                color: (currentIndex == index) ?  "#01D766" : "#FFFFFF"
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                Layout.fillHeight: true

                Rectangle {
                    anchors{left: parent.left;right: parent.right;bottom: parent.bottom}
                    height: 2
                    color: "#01D766"
                    visible: currentIndex == index
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        currentIndex = index
                    }
                }
            }
        }

        Rectangle {
            width: 2
            height: root.height*0.6
            color: "#01D766"
        }

        Label {
            height: parent.height
            text: qsTr("工具")
            color: (currentIndex == tabButtonsRepeater.count) ?  "#01D766" : "#FFFFFF"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true


            Rectangle {
                anchors{left: parent.left;right: parent.right;bottom: parent.bottom}
                height: 2
                color: "#01D766"
                visible: currentIndex == tabButtonsRepeater.count
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    currentIndex = tabButtonsRepeater.count
                }
            }
        }

        Label {
            height: parent.height
            text: qsTr("软件设置")
            color: (currentIndex == tabButtonsRepeater.count+1) ?  "#01D766" : "#FFFFFF"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true


            Rectangle {
                anchors{left: parent.left;right: parent.right;bottom: parent.bottom}
                height: 2
                color: "#01D766"
                visible: currentIndex == tabButtonsRepeater.count+1
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    currentIndex = tabButtonsRepeater.count+1
                }
            }
        }

        Label {
            height: parent.height
            text: qsTr("终端输出")
            color: (currentIndex == tabButtonsRepeater.count+2) ?  "#01D766" : "#FFFFFF"
            font.pixelSize: 12
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true


            Rectangle {
                anchors{left: parent.left;right: parent.right;bottom: parent.bottom}
                height: 2
                color: "#01D766"
                visible: currentIndex == tabButtonsRepeater.count+2
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    currentIndex = tabButtonsRepeater.count+2
                }
            }
        }
    }
}
