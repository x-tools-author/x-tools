import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/component"

Item {
    id: root

    property var _topMargin: 0
    property var _bottomMargin: 0
    property var _leftMargin: 0
    property var _rightMargin: 0

    property var _leftPannelWidth: 166
    property var _rightPannelWidth: 166
    property var _bottomPannelHeight: 137
    property var _spacing: 2

    DebugPageBlockDeviceSettings {
        id: deviceSettings
        width: _leftPannelWidth
        anchors {
            top: parent.top
            bottom: inputSettings.top
            left: parent.left
            topMargin: _topMargin
            bottomMargin: _spacing
            leftMargin: _leftMargin
        }
    }

    DebugPageBlockInputSettings {
        id: inputSettings
        width: _leftPannelWidth
        height: _bottomPannelHeight
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: _bottomMargin
            leftMargin: _leftMargin
        }
    }

    DebugPageBlockOutputtArea {
        id: outputtArea
        anchors {
            top: parent.top
            bottom: inputArea.top
            left: deviceSettings.right
            right: outputSettings.left
            topMargin: _topMargin
            bottomMargin: _spacing
            leftMargin: _spacing
            rightMargin: _spacing
        }
    }

    DebugPageBlockInputArea {
        id: inputArea
        width: 250
        height: _bottomPannelHeight
        anchors {
            bottom: parent.bottom
            left: inputSettings.right
            right: otherSettings.left
            bottomMargin: _bottomMargin
            leftMargin: _spacing
            rightMargin: _spacing
        }
    }

    DebugPageBlockOutputSettings {
        id: outputSettings
        width: _rightPannelWidth
        anchors {
            top: parent.top
            bottom: otherSettings.top
            right: parent.right
            topMargin: _topMargin
            bottomMargin: _spacing
            rightMargin: _rightMargin
        }
    }

    DebugPageBlockOtherSettings {
        id: otherSettings
        width: _rightPannelWidth
        height: _bottomPannelHeight
        anchors {
            bottom: parent.bottom
            right: parent.right
            bottomMargin: _bottomMargin
            rightMargin: _rightMargin
        }
    }
}
