import QtQuick 2.12
import QtQuick.Controls 2.12

ComboBox {
    id: root
    font.pixelSize: 12
    implicitHeight: 25
    implicitWidth: 70
    clip: true
    rightPadding: 25

    property var _leftPadding: 5
    property var _itemHeight: 25
    property var _maxPopupLength: 0
    property bool customEnable: true

    background: Rectangle {
        color: "#FF222222"
    }

    popup: Popup {
        padding: 0
        width: (root.width > _maxPopupLength) ? root.width : _maxPopupLength
        height: 25*root.count
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

            Repeater {
                id: popupRepeater
                model: root.model

                Rectangle {
                    width: (root.width > _maxPopupLength) ? root.width : _maxPopupLength
                    height: _itemHeight
                    color: itemText.text === root.currentText ? "#FF101010" : mouseArea.containsMouse ? "#FF101010" : "#FF222222"
                    Text {
                        id: itemText
                        text: modelData
                        color: "#FFFFFF"
                        leftPadding: _leftPadding
                        verticalAlignment: Qt.AlignVCenter
                        anchors {
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                        }

                        Component.onCompleted: {
                            if (itemText.width > _maxPopupLength){
                                _maxPopupLength = itemText.width+5
                            }
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
        clip: true
        verticalAlignment: Text.AlignVCenter
        leftPadding: _leftPadding
        rightPadding: root.rightPadding
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

    MouseArea {
        anchors.fill: parent
        visible: !customEnable
        cursorShape: Qt.ForbiddenCursor
    }
}
