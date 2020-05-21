import QtQuick 2.12
import QtQuick.Layouts 1.12
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
            GridLayout {
                columns: 7
                SAKLabel {
                    text: qsTr("分类名称")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 0
                }

                SAKLineEdit {
                    text: qsTr("控制类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 1
                }

                SAKButton {
                    text: qsTr("添加分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 2
                }

                SAKLabel {
                    text: qsTr("已建分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 3
                }

                SAKComboBox {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 4
                }

                SAKButton {
                    text: qsTr("删除分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 5
                }

                Item {
                    height: 1
                    Layout.fillWidth: true
                    Layout.row: 0
                    Layout.column: 6
                }

                SAKLabel {
                    text: qsTr("所属分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 0
                }

                SAKComboBox {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 1
                }

                SAKButton {
                    text: qsTr("添加条目")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 2

                    onClicked: {
                        dataPresetController.createDataPresetItem()
                        console.info("dfdfdf")
                    }
                }

                SAKLabel {
                    text: qsTr("重新命名")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 3
                }

                SAKLineEdit {
                    text: qsTr("新名称")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 4
                }

                SAKButton {
                    text: qsTr("重命名分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 5
                }

                Item {
                    height: 1
                    Layout.fillWidth: true
                    Layout.row: 1
                    Layout.column: 6
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

                Column {
                    width: 200
                    spacing: 5
                    Repeater {
                        model: dataPresetController ? dataPresetController.dataPresetItem : []
                        Label {
                            text: dataPresetItem.itemID
                            color: "red"

                            property SAKDebuggerInputTextDataPresetItem dataPresetItem: modelData
                        }
//                        Rectangle {
//                            width: 10
//                            height: 10
//                        }
                    }
                    Repeater {
                        model: 10

                        Rectangle {
                            width: 10
                            height: 10
                        }
                    }
                }
            }
        }
    }
}
