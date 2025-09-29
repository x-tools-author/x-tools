import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

Item {
    id: root
    ScrollView {
        anchors.fill: parent
        anchors.margins: 4
        ColumnLayout {
            Label {
                text: qsTr("General")
                font.bold: true
            }
            Label {
                id: dpiLabel
                text: qsTr("High dpi scale factor rounding policy (need to reboot the app)")
            }
            ControlsComboBox {
                id: hidComboBox
                textRole: "text"
                valueRole: "value"
                Layout.minimumWidth: 248
                model: ListModel {
                    id: hidListModel
                    ListElement {
                        text: qsTr("Round up for .5 and above.")
                        value: 1
                    }
                    ListElement {
                        text: qsTr("Always round up.")
                        value: 2
                    }
                    ListElement {
                        text: qsTr("Always round down.")
                        value: 3
                    }
                    ListElement {
                        text: qsTr("Round up for .75 and above.")
                        value: 4
                    }
                    ListElement {
                        text: qsTr("Don't round.")
                        value: 5
                    }
                }

                onActivated: xSettings.hdpiPolicy = hidComboBox.currentValue
                Component.onCompleted: {
                    var v = xSettings.hdpiPolicy;
                    var ret = hidComboBox.indexOfValue(v);
                    if (ret >= 0 && ret < hidComboBox.count) {
                        hidComboBox.currentIndex = ret;
                    }
                }
            }
            Label {
                text: qsTr("Language")
            }
            // model: xApp.supportedLanguages
            // onActivated: {
            //     xSettings.setLanguage(displayText)
            //     xApp.setupLanguage(displayText)
            // }
            // Layout.minimumWidth: 248
            // Component.onCompleted: {
            //     var language = xSettings.language()
            //     if (!language) {
            //         language = xApp.defaultLanguage()
            //     }
            //     var index = model.indexOf(language)
            //     currentIndex = index
            // }
            ControlsComboBox {}
            Label {
                text: qsTr("Theme")
                font.bold: true
            }
            Label {
                text: qsTr("Application theme")
            }
            ControlsComboBox {
                textRole: "text"
                valueRole: "value"
                onActivated: {
                    xMaterialTheme = currentValue;
                    xSettings.setValue(xKeysObj.materialTheme, currentValue);

                    xApp.updateWindowStyle(mainWindow, Material.backgroundColor);
                }
                Layout.minimumWidth: 248
                Component.onCompleted: {
                    var index = indexOfValue(xMaterialTheme);
                    currentIndex = index;
                    xApp.updateWindowStyle(mainWindow, Material.backgroundColor);
                }
                model: ListModel {
                    ListElement {
                        text: qsTr("Dark")
                        value: Material.Dark
                    }
                    ListElement {
                        text: qsTr("Light")
                        value: Material.Light
                    }
                }
            }
            Label {
                text: qsTr("Primary color")
            }
            Grid {
                columns: 10
                columnSpacing: 4
                rowSpacing: 4
                Repeater {
                    model: themeColors()
                    Rectangle {
                        width: 32
                        height: 32
                        color: {
                            var shade = xMaterialTheme === Material.Dark ? Material.Shade200 : Material.Shade500;
                            return Material.color(modelData, shade);
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                xMaterialPrimary = modelData;
                                xSettings.setValue(xKeysObj.materialPrimary, modelData);
                            }
                        }
                        CheckBox {
                            checked: true
                            visible: xMaterialPrimary === modelData
                            anchors.centerIn: parent
                            enabled: false
                        }
                    }
                }
            }
        }
    }

    function themeColors() {
        var tmp = [];
        tmp.push(Material.Red);
        tmp.push(Material.Pink);
        tmp.push(Material.Purple);
        tmp.push(Material.DeepPurple);
        tmp.push(Material.Indigo);
        tmp.push(Material.Blue);
        tmp.push(Material.LightBlue);
        tmp.push(Material.Cyan);
        tmp.push(Material.Teal);
        tmp.push(Material.Green);
        tmp.push(Material.LightGreen);
        tmp.push(Material.Lime);
        tmp.push(Material.Yellow);
        tmp.push(Material.Amber);
        tmp.push(Material.Orange);
        tmp.push(Material.DeepOrange);
        tmp.push(Material.Brown);
        tmp.push(Material.Grey);
        tmp.push(Material.BlueGrey);
        return tmp;
    }
}
