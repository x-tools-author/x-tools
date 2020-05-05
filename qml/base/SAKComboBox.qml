import QtQuick 2.12
import QtQuick.Controls 2.12

ComboBox {
    id: root
    font.pixelSize: 12
    implicitHeight: 25
    implicitWidth: 70

    property var _leftPadding: 5
    property var _itemHeight: 25

    background: Rectangle {
        color: "#FF222222"
    }

    popup: Popup {
        padding: 0
        width: root.width
        height: 25*(root.count+1)
        background: Rectangle {
            color: "#00000000"
        }

        Column {
            padding: 0
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            // 占位符
            Item {
                height: root.height
                width: 1
            }

            Repeater {
                model: root.count
                Rectangle {
                    width: root.width
                    height: _itemHeight
                    color: mouseArea.containsMouse ? "#FF101010" : "#FF222222"
                    Text {
                        text: root.textAt(index)
                        color: "#FFFFFF"
                        leftPadding: _leftPadding
                        verticalAlignment: Qt.AlignVCenter
                        anchors {
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            root.currentIndex = index
                            root.popup.close()
                        }
                    }
                }
            }
        }
    }

    contentItem: Label {
        text: root.currentText
        font: root.font
        color: "#FFFFFF"
        verticalAlignment: Text.AlignVCenter
        leftPadding: _leftPadding
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
    }

    indicator: Image {
        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/icons/上拉.png"
        rotation: root.popup.opened ? 0 : 180
        anchors{
            right: parent.right
            rightMargin: 5
            verticalCenter: parent.verticalCenter
        }
    }
}
