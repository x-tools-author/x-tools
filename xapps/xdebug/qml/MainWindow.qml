import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs

import xTools.xDebug

Window {
    id: mainWindow
    width: 1024
    height: 576
    visible: true
    color: "#00000000"
    title: qsTr("Easy Debug")
    flags: Qt.FramelessWindowHint | Qt.Window
    opacity: xMoYu ? 0.3 : 1.0

    property string xDisabledText: qsTr("Disable")
    property string xNoneText: qsTr("None")
    property alias xKeysObj: settingKeys
    readonly property int theme: xSettings.value(settingKeys.materialTheme, Material.Dark)
    readonly property int primary: xSettings.value(settingKeys.materialPrimary, Material.Indigo)
    property int xMaterialTheme: theme ? theme : Material.Dark
    property int xMaterialPrimary: primary ? primary : Material.Indigo
    property bool isWindowed: visibility === Window.Windowed
    readonly property string xTrue: "true"
    readonly property string xFalse: "false"
    property int xColorShade: xMaterialTheme === Material.Dark ? Material.Shade200 : Material.Shade500

    Material.theme: xMaterialTheme
    Material.accent: Material.color(xMaterialPrimary, xColorShade)
    Material.primary: Material.color(xMaterialPrimary, xColorShade)

    QtObject {
        id: settingKeys
        readonly property string materialTheme: "materialTheme"
        readonly property string materialPrimary: "materialPrimary"
        readonly property string pageIndex: "pageIndex"
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onPositionChanged: function (mouse) {
            if (mouse.x < 4) {
                cursorShape = Qt.SizeHorCursor
            } else if (mouse.x > mainWindow.width - 4) {
                cursorShape = Qt.SizeHorCursor
            } else if (mouse.y < 4) {
                cursorShape = Qt.SizeVerCursor
            } else if (mouse.y > mainWindow.height - 4) {
                cursorShape = Qt.SizeVerCursor
            }
        }
        onPressed: function (mouse) {
            if (mouse.x < 4) {
                mainWindow.startSystemResize(Qt.LeftEdge)
            } else if (mouse.x > mainWindow.width - 4) {
                mainWindow.startSystemResize(Qt.RightEdge)
            } else if (mouse.y < 4) {
                mainWindow.startSystemResize(Qt.TopEdge)
            } else if (mouse.y > mainWindow.height - 4) {
                mainWindow.startSystemResize(Qt.BottomEdge)
            }
        }
    }
    Pane {
        id: mainWindowPane
        anchors.fill: parent
        anchors.margins: mainWindow.isWindowed ? 8 : 0
        padding: 0
        antialiasing: true
        background: Rectangle {
            radius: 4
            color: Material.backgroundColor
        }
        MainWindowTitleBar {
            id: titleBar
            mainWindowVisibility: mainWindow.visibility
            height: 32
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            onInvokeClose: mainWindow.close()
            onInvokeShowWindowed: mainWindow.showNormal()
            onInvokeShowMaximized: mainWindow.showMaximized()
            onInvokeShowMinimized: mainWindow.showMinimized()
        }
        MainWindowToolBar {
            id: toolBar
            anchors.left: parent.left
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            onPageIndexChanged: {
                xSettings.setValue(settingKeys.pageIndex, pageIndex)
            }
        }
        MainWindowPages {
            id: pageStackLayout
            currentPageIndex: toolBar.pageIndex
            anchors.top: titleBar.bottom
            anchors.left: toolBar.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    MessageDialog {
        id: messageDialog
    }

    Component.onCompleted: {
        var index = xSettings.value(settingKeys.pageIndex)
        toolBar.pageIndex = index ? parseInt(index) : 0
        xApp.showSplashScreenMessage("Initializd...")
    }

    function showMessage(title, text) {
        messageDialog.title = title
        messageDialog.text = text
        messageDialog.open()
    }
}
