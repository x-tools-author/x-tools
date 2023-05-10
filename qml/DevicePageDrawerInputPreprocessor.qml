import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ED.EasyDebug
import "common"

EDGroupBox {
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
        EDLabel {
            text: qsTr("Prefix")
        }
        EDRNComboBox {
            id: prefixComboBox
            settingKey: settingKeys.inputPreprocessorPrefix
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Suffix")
        }
        EDRNComboBox {
            id: suffixComboBox
            settingKey: settingKeys.inputPreprocessorSuffix
            Layout.fillWidth: true
        }
        EDLabel {
            text: qsTr("Escape character")
        }
        EDEscComboBox {
            id: escapeCharacterComboBox
            settingKey: settingKeys.inputPreprocessorEscapeCharacter
            Layout.fillWidth: true
        }
    }
} // GroupBox
