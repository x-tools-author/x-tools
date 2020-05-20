import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"
import "qrc:/qml/component"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/设置 (1).png"
    sakTitle: qsTr("输入设置")

    property SAKDebuggerInputManager inputManager: sakdebugger ? sakdebugger.inputManager : null
    property SAKDebuggerInputSettings inputSettings: inputManager ? inputManager.inputSettings : null

    contentItem: Item{
        Grid {
            id: grid
            columns: 2
            anchors.fill: parent
            columnSpacing: 3
            rowSpacing: 8
            leftPadding: 5
            topPadding: 5

            SAKCheckBox {
                text: qsTr("定时发送")

                onCheckedChanged: {
                    if (inputSettings){
                        inputSettings.startTimer(checked)
                    }
                }
            }

            SAKLineEdit {
                placeholderText: qsTr("单位:ms")
                maximumLength: 4

                Component.onCompleted: {
                    if (inputSettings){
                        text = inputSettings.cyclicTime
                    }
                }
            }

            SAKText {
                text: qsTr("参数模型")
                height: 25
            }

            SAKComboBox {
                model: inputSettings ? inputSettings.crcParameterModel : 15
                onCurrentTextChanged: {
                    if (inputSettings){
                        inputSettings.currentCRCModel = currentText
                    }
                }
            }

            SAKText {
                text: qsTr("数据格式")
                height: 25
            }

            SAKComboBox {
                model: inputSettings ? inputSettings.textFormats : 7
                onCurrentTextChanged: {
                    if (inputSettings){
                        inputSettings.currentInputFromat = currentText
                    }
                }
            }
        }
    }
}
