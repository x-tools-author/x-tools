import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root


    /*color: Material.backgroundDimColor
    radius: xPingRadius
    border.width: 0
    Rectangle {
        color: parent.color
        height: xPingRadius
        width: height
        anchors.bottom: parent.bottom
    }
    Rectangle {
        color: parent.color
        height: xPingRadius
        width: height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }*/
    MouseArea {
        anchors.fill: parent
        onPressed: xPingMainWindow.startSystemMove()
    }

    ToolButton {
        id: xPingButton
        icon.source: "qrc:/Resources/Icons/xPing.svg"
        icon.color: xPingPrimaryColor
        display: AbstractButton.IconOnly
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            settingsMenu.popup(xPingCentralRectangle.border.width, xPingButton.height)
        }
    }
    MainWindowSettings {
        id: settingsMenu
        visible: false
    }
    Label {
        id: xPingLabel
        text: "Ping Ping" + " " + xApp.version()
        color: xPingPrimaryColor
        font.pixelSize: 16
        font.italic: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: xPingButton.right
    }

    Rectangle {
        color: closeMouseArea.containsMouse ? Material.color(Material.Red) : "#00000000"
        height: parent.height
        width: height
        anchors.right: parent.right
        anchors.rightMargin: xPingCentralRectangle.border.width
        anchors.top: parent.top
        anchors.topMargin: xPingCentralRectangle.border.width
        radius: xPingRadius
        border.width: xPingCentralRectangle.border.width
        border.color: color
        Rectangle {
            color: parent.color
            height: xPingRadius
            width: xPingRadius
        }
        Rectangle {
            color: parent.color
            height: xPingRadius
            width: xPingRadius
            anchors.bottom: parent.bottom
        }
        Rectangle {
            color: parent.color
            height: xPingRadius
            width: xPingRadius
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }
        MouseArea {
            id: closeMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: xPingMainWindow.close()
        }
        Image {
            id: closeIcon
            source: "qrc:/Resources/Icons/close.svg"
            anchors.centerIn: parent
            width: parent.width * 0.4
            height: parent.height * 0.4
            fillMode: Image.PreserveAspectFit
        }
        MultiEffect {
            source: closeIcon
            anchors.fill: closeIcon
            colorization: 1.0
            colorizationColor: "gray"
        }
    }

    GridLayout {
        id: infoGridLayout
        visible: xApp.active !== 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        columns: 8
        anchors.horizontalCenter: parent.horizontalCenter
        Label {
            id: totalLabel
            text: qsTr("Total") + ": "
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Label {
            text: xApp.total
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Label {
            text: qsTr("Active") + ": "
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Label {
            text: xApp.active
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Label {
            text: qsTr("Finished") + ": "
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        Label {
            text: xApp.finished
            verticalAlignment: Qt.AlignVCenter
            Layout.fillHeight: true
        }
        ProgressBar {
            from: 0
            to: xApp.total
            value: xApp.finished
            Layout.columnSpan: 8
            Layout.fillWidth: true
            Layout.bottomMargin: 4
        }
    }

    Item {
        id: finishedRectangle
        width: finishedLabel.width * 1.4
        height: finishedLabel.height * 1.4
        visible: false
        anchors.centerIn: parent
        Label {
            id: finishedLabel
            anchors.centerIn: parent
            text: qsTr("Ping finished")
            color: xPingGreenColor
        }
        Timer {
            id: finishedTimer
            interval: 5000
            running: true
            repeat: false
            onTriggered: {
                finishedRectangle.visible = false
            }
        }
        Connections {
            target: xApp
            function onPingFinished() {
                finishedRectangle.visible = true
                finishedTimer.restart()
            }
        }
    }
}
