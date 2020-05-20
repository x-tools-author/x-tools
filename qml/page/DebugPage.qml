import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/component"

Item {
    id: root

    property string controlPannel: ""

    property var _topMargin: 0
    property var _bottomMargin: 0
    property var _leftMargin: 0
    property var _rightMargin: 0

    property var _leftPannelWidth: 166
    property var _rightPannelWidth: 166
    property var _bottomPannelHeight: 137
    property var _spacing: 2

    property var deviceSettingsPanel: deviceSettings
    property SAKDebugger _sakDebugger: null

    DebugPageBlockDeviceSettings {
        id: deviceSettings
        sakdebugger: _sakDebugger
        width: _leftPannelWidth
        anchors {
            top: parent.top
            bottom: inputSettings.top
            left: parent.left
            topMargin: _topMargin
            bottomMargin: _spacing
            leftMargin: _leftMargin
        }

        onSakdebuggerChanged: {
            if (sakdebugger){
                setControlPannelDebugger()
            }
        }
    }

    DebugPageBlockInputSettings {
        id: inputSettings
        sakdebugger: _sakDebugger
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
        sakdebugger: _sakDebugger
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
        sakdebugger: _sakDebugger
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

        onOpenPresetDataPopupRequest: {
            presetDataPopup.open()
        }
    }

    DebugPageBlockOutputSettings {
        id: outputSettings
        sakdebugger: _sakDebugger
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
        sakdebugger: _sakDebugger
        width: _rightPannelWidth
        height: _bottomPannelHeight
        anchors {
            bottom: parent.bottom
            right: parent.right
            bottomMargin: _bottomMargin
            rightMargin: _rightMargin
        }
    }

    DebugPagePopupPresetData {
        id: presetDataPopup
        sakdebugger: _sakDebugger
        x: (root.width-presetDataPopup.width)/2
        y: (root.height-presetDataPopup.height)/2
    }

    function createControlPannel(source, debuggerType){
        deviceSettings.createControlPannel(source)
        _sakDebugger = SAKDebuggerManager.createDebugger(debuggerType)
    }
}
