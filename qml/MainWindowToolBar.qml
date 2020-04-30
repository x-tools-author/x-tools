import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("瑞士军刀")
    flags: Qt.FramelessWindowHint | Qt.Dialog

    DragHandler {
        id: resizeHandler
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
        onActiveChanged: if (active) {
            const p = resizeHandler.centroid.position;
            const b = bw + 10; // Increase the corner size slightly
            let e = 0;
            if (p.x < b) { e |= Qt.LeftEdge }
            if (p.x >= width - b) { e |= Qt.RightEdge }
            if (p.y < b) { e |= Qt.TopEdge }
            if (p.y >= height - b) { e |= Qt.BottomEdge }
            mainWindow.startSystemResize(e);
        }
    }


    Page {
        anchors.fill: parent
        background: Rectangle{color: "#222222"}
        header: ToolBar {
            id: toolBar
            height: 28
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

                    Text {
                        text: qsTr("瑞士军刀")
                        color: "#FFFFFF"
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Qt.AlignVCenter
                        font.pixelSize: 14
                        font.weight: Font.Normal
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
    }
}
