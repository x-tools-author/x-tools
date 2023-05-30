import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

EDPane {
    id: root
    ScrollView {
        anchors.fill: parent
        ColumnLayout {
            EDLabel {
                text: qsTr("General")
                font.bold: true
            }
            EDLabel {
                id: dpiLabel
                text: qsTr("High dpi scale factor rounding policy (need to reboot the app)")
            }
            EDComboBox {
                id: hidComboBox
                textRole: "text"
                valueRole: "value"
                settingKey: "highDpiScaleFactorRoundingPolicy"
                model: ListModel {
                    id: hidListModel
                    ListElement{text: qsTr(".5"); value: 1; detail: qsTr("Round up for .5 and above.")}
                    ListElement{text: qsTr("Up"); value: 2; detail: qsTr("Always round up.")}
                    ListElement{text: qsTr("Down"); value: 3; detail: qsTr("Always round down.")}
                    ListElement{text: qsTr(".75"); value: 4; detail: qsTr("Round up for .75 and above.")}
                    ListElement{text: qsTr("Don't"); value: 5; detail: qsTr("Don't round.")}
                }
                property string detail: hidListModel.get(currentIndex).detail
            }
            EDLabel {
                text: String("(%1)").arg(hidComboBox.detail)
            }
            EDLabel {
                text: qsTr("UI")
            }
            EDComboBox {
                model: [qsTr("Classical"), qsTr("Modern")]
                onActivated: {
                    if (currentIndex === 0) {
                        sakSettings.sdkSetValue("isQmlUi", edFalse)
                    } else {
                        sakSettings.sdkSetValue("isQmlUi", edTrue)
                    }
                }
                Component.onCompleted: {
                    var isQmlUi = sakSettings.sakValue("isQmlUi")
                    if (isQmlUi) {
                        currentIndex = 1
                    }
                }
            }
            EDLabel {
                text: qsTr("Language")
            }
            EDComboBox {
                textRole: "text"
                valueRole: "value"
                settingKey: "language"
                //Layout.fillWidth: true
                model: ListModel {
                    id: languageListModel
                }
                onActivated: sakI18n.installTranslator(currentValue)
            }
            EDLabel {
                text: qsTr("Font Family")
                visible: false
            }
            EDFontFamiliesComboBox {
                settingKey: "fontFamily"
                //Layout.fillWidth: true
                onActivated: sakInterface.setAppFont(currentText)
                visible: false
            }
            EDLabel {
                text: qsTr("Theme")
                font.bold: true
            }
            EDLabel {
                text: qsTr("Application theme")
            }
            EDComboBox {
                textRole: "text"
                valueRole: "value"
                settingKey: mainWindow.keysObj.materialTheme
                model: ListModel {
                    ListElement{text: qsTr("System"); value: Material.System}
                    ListElement{text: qsTr("Dark"); value: Material.Dark}
                    ListElement{text: qsTr("Light"); value: Material.Light}
                }
                onActivated: edMaterialTheme = currentValue
                Component.onCompleted: edMaterialTheme = currentValue
            }
            Repeater {
                model: [
                    [qsTr("Accent color"), EDMaterialColors.Option.MaterialAccent],
                    [qsTr("Primary color"), EDMaterialColors.Option.MaterialPrimary]
                ]
                Column {
                    EDLabel {
                        text: modelData[0]
                    }
                    EDMaterialColors { option: modelData[1] }
                }
            }
            EDLabel {
                text: qsTr("Reset accent color and primary to default")
            }
            EDButton {
                text: qsTr("Reset")
                onClicked: {
                    edMaterialPrimary = Material.color(Material.Indigo)
                    if (edMaterialTheme === Material.Dark) {
                        edMaterialAccent = Material.color(Material.Pink, Material.Shade200)
                    } else {
                        edMaterialAccent = Material.color(Material.Pink)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        var list = sakI18n.languanges
        for (var i = 0; i < list.length; i++) {
            var item = {}
            item["text"] = list[i]["friendly-name"]
            item["value"] = list[i]["name"]
            languageListModel.append(item)
        }
    }
}
