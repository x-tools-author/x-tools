import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import SAK.Custom
import "common"

EDGroupBox {
    id: storerGroupBox
    title: qsTr("Storer")

    property EDStorerTool storer: null

    QtObject {
        id: settingKeys

        readonly property string storerFileName: groupName + "/storerFileName"
        readonly property string storerEnable: groupName + "/storerEnable"
        readonly property string storerSaveDate: groupName + "/storerSaveDate"
        readonly property string storerSaveTime: groupName + "/storerSaveTime"
        readonly property string storerSaveMs: groupName + "/storerSaveMs"
        readonly property string storerTextFormat: groupName + "/storerTextFormat"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        Repeater {
            model: [
                [qsTr("Enable data storer"), settingKeys.storerEnable],
                [qsTr("Save date information"), settingKeys.storerSaveDate],
                [qsTr("Save time information"), settingKeys.storerSaveTime],
                [qsTr("Save ms information"), settingKeys.storerSaveMs],
            ]
            EDCheckBox {
                text: modelData[0]
                settingKey: modelData[1]
                Layout.columnSpan: 2
                onClicked: updateStorParameters(index, checked)
                Component.onCompleted: updateStorParameters(index, checked)

                function updateStorParameters(parameterIndex, checked) {
                    if (storer) {
                        if (parameterIndex === 0) {
                            storer.enable = checked
                        } else if (parameterIndex === 1) {
                            storer.saveDate = checked
                        } else if (parameterIndex === 2) {
                            storer.saveTime = checked
                        } else if (parameterIndex === 3) {
                            storer.saveMs = checked
                        }
                    }
                }
            }
        }

        EDLabel {
            text: qsTr("Text format")
        }
        EDTextFormatComboBox {
            id: textFormatComboBox
            settingKey: settingKeys.storerTextFormat
            Layout.fillWidth: true
            onActivated: setStorerTextFormat()
            Component.onCompleted: setStorerTextFormat()

            function setStorerTextFormat() {
                if (storer) {
                    //storer.sa setFormat(currentValue)
                }
            }
        }

        EDButton {
            text: qsTr("Select")
            Layout.columnSpan: 2
            Layout.fillWidth: true
            onClicked: fileDialog.open()
        }
        EDTextField {
            id: fileNameTextField
            settingKey: settingKeys.storerFileName
            Layout.columnSpan: 2
            Layout.fillWidth: true
            onTextEdited: setStorerFileName()
            Component.onCompleted: setStorerFileName()

            function setStorerFileName() {
                if (storer) {
                    storer.fileName = fileNameTextField.text
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: [qsTr("Text files (*.txt)"), qsTr("All files (*)")]
        onAccepted: {
            if (storer) {
                fileNameTextField.text = currentFile
                storer.setPath(currentFile)
            }
        }
    }
} // EDGroupBox
