import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

SAKPane {
    id: root
    ScrollView {
        anchors.fill: parent
        ColumnLayout {
            SAKLabel {
                text: qsTr("General")
                font.bold: true
            }
            SAKLabel {
                id: dpiLabel
                text: qsTr("High dpi scale factor rounding policy (need to reboot the app)")
            }
            SAKComboBox {
                id: hidComboBox
                textRole: "text"
                valueRole: "value"
                model: ListModel {
                    id: hidListModel
                    ListElement{text: qsTr(".5"); value: 1; detail: qsTr("Round up for .5 and above.")}
                    ListElement{text: qsTr("Up"); value: 2; detail: qsTr("Always round up.")}
                    ListElement{text: qsTr("Down"); value: 3; detail: qsTr("Always round down.")}
                    ListElement{text: qsTr(".75"); value: 4; detail: qsTr("Round up for .75 and above.")}
                    ListElement{text: qsTr("Don't"); value: 5; detail: qsTr("Don't round.")}
                }

                onActivated: sakSettings.hdpiPolicy = hidComboBox.currentValue

                property string detail: hidListModel.get(currentIndex).detail

                Component.onCompleted: {
                    var v = sakSettings.hdpiPolicy
                    var ret = hidComboBox.indexOfValue(v)
                    if (ret >= 0 && ret < hidComboBox.count) {
                        hidComboBox.currentIndex = ret
                    }
                }
            }
            SAKLabel {
                text: String("(%1)").arg(hidComboBox.detail)
            }
            SAKLabel {
                text: qsTr("UI")
            }
            SAKComboBox {
                model: [qsTr("Classical"), qsTr("Modern")]
                onActivated: {
                    if (currentIndex === 0) {
                        sakSettings.uiType = SAKSettings.UiTypeWidget
                    } else {
                        sakSettings.uiType = SAKSettings.UiTypeQml
                    }
                }
                Component.onCompleted: {
                    if (sakSettings.uiType === SAKSettings.UiTypeQml) {
                        currentIndex = 1
                    }
                }
            }
            SAKLabel {
                text: qsTr("Language")
            }
            SAKComboBox {
                textRole: "text"
                valueRole: "value"
                settingKey: "language"
                //Layout.fillWidth: true
                model: ListModel {
                    id: languageListModel
                }
                onActivated: sakI18n.installTranslator(currentValue)
            }
            SAKLabel {
                text: qsTr("Font Family")
                visible: false
            }
            SAKFontFamiliesComboBox {
                settingKey: "fontFamily"
                //Layout.fillWidth: true
                onActivated: sakInterface.setAppFont(currentText)
                visible: false
            }
            SAKLabel {
                text: qsTr("Theme")
                font.bold: true
            }
            SAKLabel {
                text: qsTr("Application theme")
            }
            SAKComboBox {
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
                    [qsTr("Accent color"), SAKMaterialColors.Option.MaterialAccent],
                    [qsTr("Primary color"), SAKMaterialColors.Option.MaterialPrimary]
                ]
                Column {
                    SAKLabel {
                        text: modelData[0]
                    }
                    SAKMaterialColors { option: modelData[1] }
                }
            }
            SAKLabel {
                text: qsTr("Reset accent color and primary to default")
            }
            SAKButton {
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
