import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import QtQuick.Controls.Material

import SAK.Custom
import "common"

Window {
    id: mainWindow

    width: 1280
    height: 720
    visible: true
    color: "#00000000"
    title: qsTr("EasyDebug")
    flags: Qt.FramelessWindowHint | Qt.Window
    opacity: 0.4

    property alias keysObj: settingKeys

    property int edMaterialTheme: {
        if (!sakSettings) {
            return Material.System
        }

        var theme = sakSettings.value(settingKeys.materialTheme)

        if (theme === Material.Dark) {

            //edMaterialAccent = Material.color(edMaterialAccent, Material.Shade200)
        }

        if (theme) {
            return theme
        } else {
            return Material.System
        }
    }
    property string edMaterialAccent: {
        if (!sakSettings) {
            return Material.color(Material.Pink)
        }

        var accent = sakSettings.value(settingKeys.materialAccent)
        if (accent) {
            return accent
        } else {
            return Material.color(Material.Pink)
        }
    }
    property string edMaterialPrimary: {
        if (!sakSettings) {
            return Material.color(Material.Pink)
        }

        var primary = sakSettings.value(settingKeys.materialPrimary)
        if (primary) {
            return primary
        } else {
            return Material.color(Material.Indigo)
        }
    }

    onEdMaterialThemeChanged: {
        if (sakSettings) {
            sakSettings.setValue(settingKeys.materialTheme, String(edMaterialTheme))
        }
    }
    onEdMaterialAccentChanged: {
        if (sakSettings) {
            sakSettings.setValue(settingKeys.materialAccent, String(edMaterialAccent))
        }
    }
    onEdMaterialPrimaryChanged: {
        if (sakSettings) {
            sakSettings.setValue(settingKeys.materialPrimary, String(edMaterialPrimary))
        }
    }

    Material.theme: edMaterialTheme
    Material.accent: edMaterialAccent
    Material.primary: edMaterialPrimary

    property bool isWindowed: visibility === Window.Windowed
    readonly property string edTrue: "true"
    readonly property string edFalse: "false"

    QtObject {
        id: settingKeys
        readonly property string materialTheme: "materialTheme"
        readonly property string materialAccent: "materialAccent"
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
        anchors.margins: isWindowed ? 8 : 0
        padding: 0
        antialiasing: true
        background: Rectangle {
            id: mainWindowPaneBackgroundRectangle
            radius: isWindowed ? 8 : 0
            border.width: 0
            layer.enabled: true
            antialiasing: true
            color: Material.background
            layer.effect: DropShadow {
                antialiasing: true
                samples: mainWindowPaneBackgroundRectangle.radius * 2
                radius: mainWindowPaneBackgroundRectangle.radius
            }
        }
        MainWindowToolBar {
            id: toolBar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            onPageIndexChanged: sakSettings.setValue(settingKeys.pageIndex, pageIndex)
            Component.onCompleted: {
                var index = sakSettings.value(settingKeys.pageIndex)
                if (index !== undefined) {
                    pageIndex = index
                }
            }
            SAKVerticalLine {
                anchors.right: parent.right
            }
        }
        MainWindowTitleBar {
            id: titleBar
            mainWindowVisibility: mainWindow.visibility
            height: 40
            anchors.top: parent.top
            anchors.left: toolBar.right
            anchors.right: parent.right
            onInvokeClose: mainWindow.close()
            onInvokeShowWindowed: mainWindow.showNormal()
            onInvokeShowMaximized: mainWindow.showMaximized()
            onInvokeShowMinimized: mainWindow.showMinimized()
        }
        MainWindowPages {
            id: pageStackLayout
            pageIndex: toolBar.pageIndex
            anchors.top: titleBar.bottom
            anchors.left: toolBar.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    } // SAKPane
}
