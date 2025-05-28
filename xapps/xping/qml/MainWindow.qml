import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

Window {
    id: xPingMainWindow
    visible: true
    width: 980
    height: 520
    title: qsTr("Ping Ping")
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "#00000000"
    opacity: xMagic

    signal invokeChangeLanguage(string code)

    property var xPingCentralRectangle: centralRectangle
    property alias xPingSettingKeys: settingKeys
    property int xPingTableViewRow: 0
    property var xPingTableView: mainWindowTableView
    property var redColor: Qt.color("red")
    property int xPingTheme: {
        if (xSettings.value(xPingSettingKeys.theme)) {
            return xSettings.value(xPingSettingKeys.theme)
        } else {
            return Material.Light
        }
    }
    property int xPingPrimary: {
        if (xSettings.value(xPingSettingKeys.primary)) {
            return xSettings.value(xPingSettingKeys.primary)
        } else {
            return Material.Indigo
        }
    }
    property string xPingPrimaryColor: {
        if (!xPingPrimary) {
            return Material.primary
        }

        if (typeof xPingPrimary === 'number') {
            if (xPingTheme == Material.Light) {
                return Material.color(xPingPrimary, Material.Shade500)
            } else {
                return Material.color(xPingPrimary, Material.Shade200)
            }
        } else {
            if (xPingTheme == Material.Light) {
                return Material.color(Material.Indigo, Material.Shade500)
            } else {
                return Material.color(Material.Indigo, Material.Shade200)
            }
        }
    }
    property string xPingGreenColor: {
        if (xPingTheme == Material.Light) {
            return Material.color(Material.Green, Material.Shade500)
        } else {
            return Material.color(Material.Green, Material.Shade200)
        }
    }
    property string xPingRedColor: {
        if (xPingTheme == Material.Light) {
            return Material.color(Material.Red, Material.Shade500)
        } else {
            return Material.color(Material.Red, Material.Shade200)
        }
    }

    Material.theme: xPingTheme
    Material.accent: xPingPrimary
    Material.primary: xPingPrimary
    Material.roundedScale: Material.ExtraSmallScale

    property int xPingRadius: 8
    QtObject {
        id: settingKeys
        readonly property string theme: "theme"
        readonly property string primary: "primary"
    }

    Rectangle {
        id: centralRectangle
        color: Material.backgroundColor
        anchors.fill: parent
        radius: xPingRadius
        border.color: "#22000000"
        border.width: 2
        GridLayout {
            columns: 3
            rowSpacing: 0
            columnSpacing: 0
            anchors.fill: parent
            MainWindowTitleBar {
                Layout.minimumHeight: 40
                Layout.fillWidth: true
                Layout.columnSpan: 3
            }
            Rectangle {
                color: xPingPrimaryColor
                Layout.minimumHeight: 4
                Layout.fillWidth: true
                Layout.columnSpan: 3
                Layout.leftMargin: centralRectangle.border.width
                Layout.rightMargin: centralRectangle.border.width
            }
            MainWindowController {
                Layout.minimumWidth: 210
                Layout.fillHeight: true
            }
            Rectangle {
                color: xPingPrimaryColor
                Layout.minimumWidth: 4
                Layout.fillHeight: true
                Layout.bottomMargin: centralRectangle.border.width
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                MainWindowTableView {
                    id: mainWindowTableView
                    anchors.fill: parent
                    anchors.margins: 12
                }
            }
        }

        MessageDialog {
            id: messageDialog
            title: messageType == 0 ? qsTr("Warning") : qsTr("Error")
            modality: Qt.WindowModal
            buttons: MessageDialog.Ok

            property int messageType: 0
        }
    }

    function showMessage(msg, type) {
        messageDialog.text = msg
        messageDialog.messageType = type
        messageDialog.open()
    }
}
