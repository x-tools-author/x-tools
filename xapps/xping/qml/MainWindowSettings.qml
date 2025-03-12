import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Qt.labs.qmlmodels

Menu {
    id: root

    property string lanugage: {
        var tmp = xSettings.value(settingKeys.language)
        if (!tmp) {
            tmp = xApp.defaultLanguage()
        }
        return tmp
    }

    QtObject {
        id: settingKeys
        readonly property string language: "language"
    }
    ButtonGroup {
        id: languageActionGroup
    }
    Menu {
        title: qsTr("Language")
        Repeater {
            model: xApp.supportedLanguages
            MenuItem {
                id: languageAction
                text: modelData
                checkable: true
                onTriggered: {
                    xApp.setupLanguage(modelData)
                    xSettings.setValue(settingKeys.language, modelData)
                }
                Component.onCompleted: {
                    if (root.lanugage === modelData) {
                        checked = true
                    }
                    languageActionGroup.addButton(languageAction)
                }
            }
        }
    }
    Menu {
        title: qsTr("Theme")
        Action {
            id: lightAction
            text: qsTr("Light")
            checkable: true
            onTriggered: {
                xPingTheme = Material.Light
                xSettings.setValue(xPingSettingKeys.theme, Material.Light)
            }
            Component.onCompleted: {
                if (xPingTheme === Material.Light) {
                    checked = true
                }
            }
        }
        Action {
            id: darkAction
            checkable: true
            text: qsTr("Dark")
            onTriggered: {
                xPingTheme = Material.Dark
                xSettings.setValue(xPingSettingKeys.theme, Material.Dark)
            }
            Component.onCompleted: {
                if (xPingTheme === Material.Dark) {
                    checked = true
                }
            }
        }
        ActionGroup {
            actions: [lightAction, darkAction]
        }
    }
    Menu {
        title: qsTr("Theme Color")
        Repeater {
            model: materialColors()
            MenuItem {
                id: primaryColorMenuItem
                text: {
                    var txt = ""
                    if (xPingTheme === Material.Light) {
                        txt = Material.color(modelData, Material.Shade500)
                    } else {
                        txt = Material.color(modelData, Material.Shade200)
                    }

                    return xPingPrimary === modelData ? "✔ " + txt : txt
                }
                onTriggered: {
                    xPingPrimary = modelData
                    xSettings.setValue(xPingSettingKeys.primary, modelData)
                }
                background: Rectangle {
                    color: {
                        if (xPingTheme === Material.Light) {
                            return Material.color(modelData, Material.Shade500)
                        } else {
                            return Material.color(modelData, Material.Shade200)
                        }
                    }
                }
                Component.onCompleted: {
                    if (xSettings.value(xPingSettingKeys.primary) === modelData) {
                        xPingPrimary = modelData
                    }
                }
            }
        }
    }
    MenuSeparator {}
    Action {
        checkable: true
        text: qsTr("Finished Beep")
        onTriggered: xApp.beepEnable = checked
        Component.onCompleted: checked = xApp.beepEnable
    }
    MenuSeparator {}
    Action {
        text: qsTr("Open Settings Path")
        onTriggered: {
            xSettings.openSettingsFileDir()
        }
    }
    MenuSeparator {}
    Action {
        text: qsTr("Exit")
        onTriggered: Qt.quit()
    }

    function materialColors() {
        var tmp = []
        tmp.push(Material.Red)
        tmp.push(Material.Pink)
        tmp.push(Material.Purple)
        tmp.push(Material.DeepPurple)
        tmp.push(Material.Indigo)
        tmp.push(Material.Blue)
        tmp.push(Material.LightBlue)
        tmp.push(Material.Cyan)
        tmp.push(Material.Teal)
        tmp.push(Material.Green)
        tmp.push(Material.LightGreen)
        tmp.push(Material.Lime)
        tmp.push(Material.Yellow)
        tmp.push(Material.Amber)
        tmp.push(Material.Orange)
        tmp.push(Material.DeepOrange)
        tmp.push(Material.Brown)
        tmp.push(Material.Grey)
        tmp.push(Material.BlueGrey)
        return tmp
    }
}
