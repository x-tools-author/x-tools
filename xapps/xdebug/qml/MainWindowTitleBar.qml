import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: root

    property int mainWindowVisibility: Window.Windowed

    signal invokeShowMaximized
    signal invokeShowMinimized
    signal invokeClose
    signal invokeShowWindowed

    CommonSeparator {
        edge: Qt.BottomEdge
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            mainWindow.startSystemMove();
        }
    }

    Image {
        id: logo
        height: root.height * 0.8
        width: root.height * 0.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 4
        source: "qrc:/res/icons/logo.svg"
    }
    MultiEffect {
        source: logo
        anchors.fill: logo
        colorization: 1.0
        colorizationColor: Material.primary
    }

    Text {
        text: "Easy Debug" + " " + xDebugVersion
        color: Material.primary
        font.bold: true
        font.pixelSize: 12
        font.italic: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -1
        anchors.left: logo.right
        anchors.leftMargin: 8
    }

    Row {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        Repeater {
            model: ["qrc:/res/icons/minimized.svg", "qrc:/res/icons/windowed.svg", "qrc:/res/icons/maximized.svg", "qrc:/res/icons/close.svg"]
            Rectangle {
                id: bt
                height: root.height
                width: height * 1.4
                radius: index === 3 ? mainWindowVisibility === Window.Windowed ? 4 : 0 : 0
                visible: {
                    if (index === 1) {
                        return root.mainWindowVisibility !== Window.Windowed;
                    } else if (index === 2) {
                        return root.mainWindowVisibility !== Window.Maximized;
                    }

                    return true;
                }

                color: {
                    if (index === 3) {
                        return btMouse.containsMouse ? "red" : "#00000000";
                    }

                    return btMouse.containsMouse ? Material.accent : "#00000000";
                }
                Rectangle {
                    width: parent.radius
                    anchors.left: parent.left
                    height: parent.height
                    color: parent.color
                    visible: index === 3
                }
                Rectangle {
                    height: parent.radius
                    color: parent.color
                    width: parent.width
                    anchors.bottom: parent.bottom
                    visible: index === 3
                }

                MouseArea {
                    id: btMouse
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        if (index === 0) {
                            root.invokeShowMinimized();
                        } else if (index === 1) {
                            root.invokeShowWindowed();
                        } else if (index === 2) {
                            root.invokeShowMaximized();
                        } else if (index === 3) {
                            root.invokeClose();
                        }
                    }
                }
                Image {
                    anchors.centerIn: parent
                    height: parent.height * 0.6
                    width: height
                    source: modelData
                    //mipmap: true
                    fillMode: Image.PreserveAspectFit
                    //color: Material.foreground
                }
            }
        }
    }
}
