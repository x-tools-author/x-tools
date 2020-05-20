import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/输入 (1).png"
    sakTitle: qsTr("数据输入")

    property SAKDebuggerInputManager inputManager: sakdebugger ? sakdebugger.inputManager : null
    property SAKDebuggerTextInput textInput: inputManager ? inputManager.textInput: null
    property SAKDebuggerInputSettings inputSettings: inputManager ? inputManager.inputSettings : null

    signal openPresetDataPopupRequest()

    Connections {
        target: inputSettings
        onWriteBytesRequest: {
            if (textInput){
                textInput.writeRawData(textinputArea.text)
            }
        }
    }

    contentItem: Item{
        SAKTextArea {
            id: textinputArea
            height: 58
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 5
                leftMargin: 5
                rightMargin: 5
            }

            onTextChanged: {
                var ret = textInput.formattingInputText(text)
                textinputArea.text = ret
                textinputArea.cursorPosition = textinputArea.length
                console.info(ret)
            }
        }

        Row {
            id: buttonRow
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            rightPadding: 5
            bottomPadding: 5
            spacing: 5

            Repeater {
                model: [qsTr("清空输入"), qsTr("数据预设"), qsTr("发送预设"), qsTr("发送数据")]

                SAKButton {
                    height: 25
                    text: modelData

                    onClicked: {
                        if (!(inputSettings && textInput)){
                            return
                        }

                        if (index == 0){
                            textinputArea.clear()
                        }

                        if (index == 1){
                            openPresetDataPopupRequest()
                        }

                        if (index == 3){
                            textInput.writeBytes(textinputArea.text)
                        }
                    }
                }
            }
        }

        Row {
            anchors.verticalCenter: buttonRow.verticalCenter
            anchors.left: parent.left
            leftPadding: 5
            spacing: 5

            Repeater {
                model: [qsTr("追加CRC"), qsTr("大端CRC")]
                SAKCheckBox {
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData

                    onCheckedChanged: {
                        if (textInput){
                            if (index == 0){
                                textInput.addCRCFlag = checked
                            }

                            if (index == 1){
                                textInput.bigEndianCRCFlag = checked
                            }
                        }
                    }

                    Component.onCompleted: {
                        if (textInput){
                            if (index == 0){
                                checked = textInput.addCRCFlag
                            }

                            if (index == 1){
                                checked = textInput.bigEndianCRCFlag
                            }
                        }
                    }
                }
            }
        }
    }
}
