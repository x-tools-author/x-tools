import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "../common"

SAKGroupBox {
    id: root
    title: qsTr("Data preprocessor")

    property alias prefix: prefixComboBox.currentValue
    property alias suffix: suffixComboBox.currentValue
    property alias escapeCharacter: escapeCharacterComboBox.currentValue

    QtObject {
        id: settingKeys
        readonly property string inputPreprocessorPrefix: groupName + "/inputPreprocessorPrefix"
        readonly property string inputPreprocessorSuffix: groupName + "/inputPreprocessorSuffix"
        readonly property string inputPreprocessorEscapeCharacter: groupName + "/inputPreprocessorEscapeCharacter"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        SAKLabel {
            text: qsTr("Prefix")
        }
        SAKRNComboBox {
            id: prefixComboBox
            settingKey: settingKeys.inputPreprocessorPrefix
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("Suffix")
        }
        SAKRNComboBox {
            id: suffixComboBox
            settingKey: settingKeys.inputPreprocessorSuffix
            Layout.fillWidth: true
        }
        SAKLabel {
            text: qsTr("Escape character")
        }
        SAKEscComboBox {
            id: escapeCharacterComboBox
            settingKey: settingKeys.inputPreprocessorEscapeCharacter
            Layout.fillWidth: true
        }
    }
} // GroupBox
