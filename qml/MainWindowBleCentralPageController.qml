import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SAK.Custom
import "common"

EDPane {
    id: root
    padding: 0

    property alias filtter: filtterTextField.text
    property alias timerout: timeoutSpinBox.text

    property alias currentServiceIndex: serviceComboBox.currentIndex
    property alias currentCharacteristicIndex: characteristicsComboBox.currentIndex
    property alias writeModel: writtingComboBox.currentIndex
    property alias serviceNames: serviceComboBox.model
    property alias characteristicNames: characteristicsComboBox.model

    signal invokeChangedNotify()
    signal invokeRead()

    QtObject {
        id: settingKeys
        readonly property string filter: groupName + "/filter"
        readonly property string timeout: groupName + "/timeout"
    }

    GridLayout {
        id: contentGridLayout
        columns: 2
        anchors.fill: parent
        EDLabel {
            text: qsTr("BLE central settings")
            Layout.columnSpan: 2
        }
        EDLabel {
            text: qsTr("Device list")
        }
        EDbleComboBox {
            id: namesComboBox
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("filter")
        }
        EDTextField {
            id: filtterTextField
            settingKey: settingKeys.filter
            Layout.fillWidth: true
            onTextEdited: namesComboBox.bleController.filtter = text
        }
        EDLabel {
            text: qsTr("Timeout")
        }
        EDTextField {
            id: timeoutSpinBox
            settingKey: settingKeys.timeout
            Layout.fillWidth: true
            onTextEdited: namesComboBox.bleController.timeout = Number(text)
        }
        EDLabel {
            text: qsTr("Services")
        }
        EDComboBox {
            id: serviceComboBox
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Characteristics")
        }
        EDComboBox {
            id: characteristicsComboBox
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Writting")
        }
        EDComboBox {
            id: writtingComboBox
            model: [qsTr("Write"), qsTr("Write without Response")]
            Layout.fillWidth: true
        }
        Grid {
            id: btGrid
            columns: 2
            spacing: contentGridLayout.columnSpacing
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Repeater {
                id: btRepeater
                model: [
                    qsTr("Notify"), qsTr("Read"),
                ]
                EDButton {
                    text: modelData
                    width: (parent.width - btGrid.spacing)/2
                    onClicked: {
                        if (index === 0) {
                            root.invokeChangedNotify()
                        } else {
                            root.invokeRead()
                        }
                    }
                }

                function setNotifyText(notified) {
                    var bt = btRepeater.itemAt(0)
                    bt.text = notified ? qsTr("Unnotify") : qsTR("Notify")
                }
            }
        }
    }

    function setNotifyText(notified) {
        btGrid.setNotifyText(notified)
    }
}
