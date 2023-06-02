import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "common"

ScrollView {
    id: root
    contentWidth: -1

    property alias prefix: preprocessorGroupBox.prefix
    property alias suffix: preprocessorGroupBox.suffix
    property alias escapeCharacter: preprocessorGroupBox.escapeCharacter

    property alias crcType: crcGroupBox.crcType
    property alias crcStartIndex: crcGroupBox.crcStartIndex
    property alias crcEndIndex: crcGroupBox.crcEndIndex
    property alias crcEnable: crcGroupBox.crcEnable

    ColumnLayout {
        width: root.width
        DevicePageDrawerInputPreprocessor {
            id: preprocessorGroupBox
            Layout.fillWidth: true
        }
        DevicePageDrawerInputCrc {
            id: crcGroupBox
            Layout.fillWidth: true
        }
    }
}
