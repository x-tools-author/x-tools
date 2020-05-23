import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

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
                    text: qsTr("分组名称")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 0
                }

                SAKLineEdit {
                    id: groupNameLineEdit
                    text: ""
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 1
                }

                SAKButton {
                    text: qsTr("添加分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 2

                    onClicked: {
                        dataPresetController.addGroup(groupNameLineEdit.text)
                    }
                }

                SAKLabel {
                    text: qsTr("已建分组")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 3
                }

                SAKComboBox {
                    id: groupsComboBox
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 4
                    model: dataPresetController.groups
                }

                SAKButton {
                    text: qsTr("删除分类")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 0
                    Layout.column: 5

                    onClicked: {
                        dataPresetController.removeGroup(groupsComboBox.currentText)
                    }
                }

                Item {
                    height: 1
                    Layout.fillWidth: true
                    Layout.row: 0
                    Layout.column: 6
                }

                SAKLabel {
                    text: qsTr("所属分组")
                    Layout.alignment: Qt.AlignVCenter
                    Layout.row: 1
                    Layout.column: 0
                }

                SAKComboBox {
                    model: dataPresetController.groups
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

        Rectangle {
            color: "#10ffffff"
            anchors {
                top: topItem.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                topMargin: 5
            }

            ListView {
                id: dataPresetItemListView
                clip: true
                model: dataPresetController.dataPresetItems
                spacing: 5
                anchors.fill: parent
                highlight: Rectangle {
                    color: "#20ffffff"
                }
                delegate: Item {
                    id: dataPresetItemItem
                    width: parent.width
                    height: dataPresetItemGridLayout.height + 12

                    property SAKDebuggerInputTextDataPresetItem dataPresetItem: modelData

                    Rectangle {
                        id:  dataPresetItemInfoRect
                        color: "#161616"
                        anchors {
                            fill: parent
                            margins: 6
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: dataPresetItemListView.currentIndex = index
                        }

                        GridLayout {
                            id: dataPresetItemGridLayout
                            width: parent.width
                            columns: 8

                            SAKLabel {
                                text: qsTr("所属分类")
                            }

                            SAKComboBox {

                            }

                            SAKLabel {
                                text: qsTr("数据格式")
                            }

                            SAKComboBox {
                                model: dataPresetItemItem.dataPresetItem.formats
                            }

                            SAKLabel {
                                text: qsTr("名称")
                            }

                            SAKLineEdit {
                                Component.onCompleted: text = dataPresetItemItem.dataPresetItem.itemName
                            }

                            SAKButton {
                                text: qsTr("修改名称")
                            }

                            SAKLabel {
                                Layout.fillWidth: true
                                height: 1
                                text: " "
                            }

                            SAKTextArea {
                                Layout.fillWidth: true
                                Layout.columnSpan: 8
                                implicitHeight: 50

                                Component.onCompleted: {
                                    text = dataPresetItemItem.dataPresetItem.itemText
                                }
                            }
                        }
                    }

                    DropShadow {
                        anchors.fill: dataPresetItemInfoRect
                        horizontalOffset: 0
                        verticalOffset: 0
                        radius: 8.0
                        samples: 20
                        color: "#90000000"
                        source: dataPresetItemInfoRect
                    }
                }
            }
        }
    }
}
