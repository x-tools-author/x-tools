import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ToolBar {
    id: toolBar
    height: 28
    property alias title: titleLabel.text

    background: Rectangle {
        color: "#161616"
        anchors.fill: toolBar

        MouseArea {
            anchors.fill: parent

            property bool moveEffective: false
            property real pressedX: 0
            property real pressedY: 0

            onPressed: {
                moveEffective = true
                pressedX = mouseX
                pressedY = mouseY
            }

            onMouseXChanged: {
                if (moveEffective){
                    mainWindow.setX(mainWindow.x - (pressedX-mouseX))
                    mainWindow.setY(mainWindow.y - (pressedY-mouseY))
                }
            }

            onReleased: {
                moveEffective = false
            }
        }

        Row {
            spacing: 5
            anchors{left: parent.left;leftMargin: 10; verticalCenter: parent.verticalCenter}
            Image {
                source: "qrc:/resources/icons/矢量图军用刀制品.png"
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                id: titleLabel
                text: qsTr("瑞士军刀")
                color: "#FFFFFF"
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Qt.AlignVCenter
                font.pixelSize: 14
                font.weight: Font.Normal
                font.bold: true
            }
        }

        Row {
            anchors.right: parent.right
            Repeater {
                model: 3

                Rectangle {
                    height: toolBar.height
                    width: height
                    color: mouseArea.containsMouse ? (index == 2 ? "red" : "#30FFFFFF")  : "#00000000"

                    Text {
                        anchors.centerIn: parent
                        text: index == 0 ? "🗕" : (index == 1 ? (mainWindow.visibility === Window.Maximized ? "🗗" : "🗖") : "🗙")
                        color: "#FFFFFF"
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            switch(index){
                            case 0:
                                mainWindow.showMinimized()
                                break;
                            case 1:
                                if(mainWindow.visibility === Window.Maximized){
                                    mainWindow.showNormal()
                                }else{
                                    mainWindow.showMaximized()
                                }
                                break
                            case 2:
                                mainWindow.close()
                            }
                        }
                    }
                }
            }
        }
    }
}
