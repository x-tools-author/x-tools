import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/输出.png"
    sakTitle: qsTr("数据输出")

    property SAKDebuggerOutputManager outputManager: sakdebugger ? sakdebugger.outputManager : null
    property SAKDebuggerTextOutput textOutput: outputManager ? outputManager.textOutput : null
    property SAKDebuggerOutputSettings outputSettings: outputManager ? outputManager.outputSettings : null

    Connections {
        target: outputSettings

        onClearOutputReauest: {
            outputTextArea.clear()
        }

        onSaveOutputDataRequest: {
            if (textOutput){
                textOutput.saveDataToFile(outputTextArea.getText(0, outputTextArea.length-1))
            }
        }
    }

    contentItem: Item{
        ScrollView {
            clip: true
            anchors {
                margins: 5
                fill: parent
            }
            SAKTextArea {
                id: outputTextArea
                implicitWidth: parent.width
                implicitHeight: parent.height
                readOnly: true
                textFormat: Text.RichText
            }
        }
    }

    Connections {
        target: textOutput
        onOutputTextRequest: {
            // 最多保存1M文本
            if (outputTextArea.length > 1024){
                outputTextArea.remove(0, outputTextArea.length-1024)
            }
            outputTextArea.append(text)
        }
    }
}
