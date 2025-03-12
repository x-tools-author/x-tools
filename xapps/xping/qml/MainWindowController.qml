import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    QtObject {
        id: settingKeys
        readonly property string startIp: "startIp"
        readonly property string endIp: "endIp"
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        Label {
            text: qsTr("Start IP address")
        }
        TextField {
            id: startIpTextField
            enabled: xApp.active === 0
            text: "192.168.1.0"
            Layout.fillWidth: true
            placeholderText: "IPv4"
            onTextEdited: xSettings.setValue(settingKeys.startIp, text)
            Component.onCompleted: {
                var startIp = xSettings.value(settingKeys.startIp)
                if (startIp) {
                    startIpTextField.text = startIp
                }
            }


            /*validator: RegularExpressionValidator {
                regularExpression: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
            }*/
        }
        Label {
            text: qsTr("End IP address")
        }
        TextField {
            id: endIpTextField
            enabled: xApp.active === 0
            text: "192.168.1.255"
            placeholderText: "IPv4"
            Layout.fillWidth: true
            onTextEdited: xSettings.setValue(settingKeys.endIp, text)
            Component.onCompleted: {
                var endIp = xSettings.value(settingKeys.endIp)
                if (endIp) {
                    endIpTextField.text = endIp
                }
            }
        }
        RadioButton {
            id: allHostRadioButton
            text: qsTr("All Host")
            checked: true
            Layout.fillWidth: true
            onClicked: {
                xApp.setFilterText("")
                xPingTableView.resetTableViewY()
            }
        }
        RadioButton {
            id: onlineHostRadioButton
            text: qsTr("Online Host")
            Layout.fillWidth: true
            onClicked: {
                xApp.setFilterText(xApp.onlineText())
                xPingTableView.resetTableViewY()
            }
        }
        RadioButton {
            id: offlineHostRadioButton
            text: qsTr("Offline Host")
            Layout.fillWidth: true
            onClicked: {
                xApp.setFilterText(xApp.offlineText())
                xPingTableView.resetTableViewY()
            }
        }
        RadioButton {
            id: deltaHostRadioButton
            text: qsTr("Delta Host")
            Layout.fillWidth: true
            onClicked: {
                xApp.setFilterTextDelta()
                xPingTableView.resetTableViewY()
            }
        }
        Button {
            text: qsTr("Start")
            enabled: xApp.active === 0
            Layout.fillWidth: true
            highlighted: true
            Material.background: xPingPrimaryColor
            Material.roundedScale: Material.ExtraSmallScale
            onClicked: {
                var startIp = startIpTextField.text
                var endIp = endIpTextField.text
                var errorString = xApp.checkParameters(startIp, endIp)
                if (errorString !== "") {
                    showMessage(errorString, 1)
                    return
                }

                xApp.startPing(startIp, endIp)
                xPingTableView.resetTableViewY()

                if (allHostRadioButton.checked) {
                    xApp.setFilterText("")
                } else if (onlineHostRadioButton.checked) {
                    xApp.setFilterText(xApp.onlineText())
                } else if (offlineHostRadioButton.checked) {
                    xApp.setFilterText(xApp.offlineText())
                }
            }
        }

        Label {
            Layout.fillHeight: true
        }
    }
    ButtonGroup {
        buttons: [allHostRadioButton, onlineHostRadioButton, offlineHostRadioButton, deltaHostRadioButton]
    }
}
