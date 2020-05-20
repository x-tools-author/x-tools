import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/base"

SAKPopup {
    id: root
    width: 600
    height: 450

    property SAKDebugger sakdebugger : null
    property SAKDebuggerInputManager inputManager: sakdebugger ? sakdebugger.inputManager : null
    property SAKDebuggerTextInput textInput: inputManager ? inputManager.textInput : null
    property SAKDebuggerInputTextDataPresetController dataPresetController: textInput ? textInput.dataPresetController : null

    contentItem: Item {
        Item {
            id: topItem
            height: 60
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
            }
            Row {
                id: addRow
                spacing: 5
                SAKLabel {
                    text: qsTr("分类名称")
                    anchors.verticalCenter: parent.verticalCenter
                }

                SAKLineEdit {
                    text: qsTr("控制类")
                    anchors.verticalCenter: parent.verticalCenter
                }

                SAKButton {
                    text: qsTr("添加分类")
                    anchors.verticalCenter: parent.verticalCenter
                }

                SAKLabel {
                    text: qsTr("已建分类")
                    anchors.verticalCenter: parent.verticalCenter
                }

                SAKComboBox {
                    anchors.verticalCenter: parent.verticalCenter
                }

                SAKButton {
                    text: qsTr("删除分类")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                id: newRow
                spacing: 5
                anchors {
                    top: addRow.bottom
                    topMargin: 5
                }
                SAKLabel {
                    text: qsTr("所属分类")
                    anchors.verticalCenter: parent.verticalCenter
                }
                SAKComboBox {
                    anchors.verticalCenter: parent.verticalCenter

                }
                SAKButton {
                    text: qsTr("添加条目")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Item {
            anchors {
                top: topItem.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                topMargin: 5
            }

            ScrollView {
                anchors.fill: parent
                background: Rectangle{
                    color: "#222222"
                }
            }
        }
    }
}
