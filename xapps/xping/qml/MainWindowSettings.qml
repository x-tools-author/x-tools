import QtQuick
import QtQuick.Controls.Material

Menu {
    id: root

    ButtonGroup {
        id: languageActionGroup
    }
    Menu {
        title: qsTr("Language")
        Repeater {
            model: [{
                    "text": "English",
                    "code": "en"
                }, {
                    "text": "简体中文",
                    "code": "zh_CN"
                }]
            MenuItem {
                id: languageAction
                text: modelData["text"]
                checkable: true
                onTriggered: xPingMainWindow.invoke //xApp.setupLanguage(modelData["code"])
                Component.onCompleted: {
                    if (xApp.language() === modelData["code"]) {
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
