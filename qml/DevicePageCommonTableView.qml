import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

import "common"

Item {
    id: root

    //Material.elevation: 8

    signal invokeEdit()
    signal invokeAppend()
    signal itemsChanged()

    property int currentRowIndex: -1
    property alias tableMode: tableView.model
    property alias tableHeaders: headerRepeater.model
    property var fillWidthColumns: []
    property var showColumns: []

    readonly property int cellPadding: 4

    onWidthChanged: tableView.forceLayout()

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id: headerRowLayout
            spacing: 0
            Layout.fillWidth: true
            Repeater {
                id: headerRepeater
                SAKLabel {
                    text: modelData
                    padding: cellPadding
                    font.bold: true
                    Layout.fillWidth: {
                        for (var i = 0; i < fillWidthColumns.length; i++) {
                            if (index === fillWidthColumns[i]) {
                                return true
                            }
                        }

                        return false
                    }
                    visible: {
                        for (var i = 0; i < showColumns.length; i++) {
                            if (index === showColumns[i]) {
                                return true
                            }
                        }

                        return false
                    }
                }
            }
        }
        Item {
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            TableView {
                id: tableView
                anchors.fill: parent
                contentWidth: -1
                delegate: Item {
                    implicitHeight: 40
//                    color: if (currentRowIndex === row) {
//                               return Material.accent
//                           } else {
//                                if (row % 2) {
//                                    return Material.color(Material.Grey)
//                                } else {
//                                    return Material.color(Material.BlueGrey)
//                                }
//                           }
//                    color: {
//                        if (currentRowIndex === row) {
//                            return Material.color(Material.accent, Material.Shade100)
//                        } else {
//                            if (row % 2) {
//                                if (edMaterialTheme === Material.Dark) {
//                                    Material.color(Material.Grey, Material.Shade700)
//                                } else {
//                                    Material.color(Material.Grey, Material.Shade100)
//                                }
//                            } else {
//                                if (edMaterialTheme === Material.Dark) {
//                                    Material.color(Material.Grey, Material.Shade800)
//                                } else {
//                                    Material.color(Material.Grey, Material.Shade200)
//                                }
//                            }
//                        }
//                    }
                    clip: true
                    SAKLabel {
                        text: display
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: cellPadding
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: currentRowIndex = row
                        onDoubleClicked: root.invokeEdit()
                    }
                }
                columnWidthProvider: function (index) {
                    var children = headerRowLayout.children
                    if (children) {
                        var item = children[index]
                        if (item && item.visible) {
                            return item.width
                        }
                    }

                    return 0
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Item { Layout.fillWidth: true }
            Repeater {
                model: [
                    qsTr("Edit"),
                    qsTr("Delete"),
                    qsTr("Clear"),
                    qsTr("Import"),
                    qsTr("Export"),
                    qsTr("Append")
                ]
                SAKButton {
                    text: modelData
                    onClicked: {
                        if (index === 0) {
                            if (currentRowIndex == -1) {
                                noSelectionMessageDialog.open()
                            } else {
                                root.invokeEdit()
                            }
                        } else if (index === 1) {
                            if (currentRowIndex == -1) {
                                noSelectionMessageDialog.open()
                            } else {
                                removeConfimMessageDialog.open()
                            }
                        } else if (index === 2) {
                            clearMessageDialog.open()
                        } else if (index === 3) {
                            fileDialog.fileMode = FileDialog.OpenFile
                            fileDialog.open()
                        } else if (index === 4) {
                            fileDialog.fileMode = FileDialog.SaveFile
                            fileDialog.open()
                        } else if (index === 5) {
                            root.invokeAppend()
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
        nameFilters: ["Json file (*.json)", "All files (*)"]
        onAccepted: {
            if (fileMode === FileDialog.SaveFile) {
                sakInterface.tableModel2jsonFile(tableMode, selectedFile)
            } else {
                sakInterface.jsonFile2tableModel(tableMode, selectedFile)
                root.itemsChanged()
            }
        }
    }

    MessageDialog {
        id: clearMessageDialog
        title: qsTr("Clear Items")
        text: qsTr("The items will be clear, please confirm the operaion.")
        informativeText: qsTr("The items will be clear and it will be remove from the settings file. You can backup the items by \"Export\" operation.")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            sakInterface.clearTableModel(tableMode)
            currentRowIndex = -1
            root.itemsChanged()
        }
        onRejected: {}
    }

    MessageDialog {
        id: removeConfimMessageDialog
        title: qsTr("Remove a Item")
        text: qsTr("The item will be removed, please confirm the operation.")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            sakInterface.removeTableModelRow(tableMode, currentRowIndex)
            currentRowIndex = -1
            root.itemsChanged()
        }
    }

    MessageDialog {
        id: noSelectionMessageDialog
        title: qsTr("Please Select an Item")
        text: qsTr("Please select an item fist, the operaion need you to specify an item.")
        buttons: MessageDialog.Ok
    }
}
