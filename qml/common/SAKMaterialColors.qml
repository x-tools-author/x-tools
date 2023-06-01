import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

GridLayout {
    id: root
    columns: 10

    property string specifyColor: ""
    property var option: EDMaterialColors.Option.MaterialUnknow

    enum Option {
        MaterialUnknow,
        MaterialAccent,
        MaterialPrimary,
        MaterialForeground,
        MaterialBackground
    }

    Connections {
        target: mainWindow
        function onEdMaterialThemeChanged() {
            colorRepeater.model = colorModel()
        }
    }

    Repeater {
        id: colorRepeater
        EDButton {
            Rectangle {
                id: colorRectangle
                anchors.fill: parent
                color: edColor(modelData, option === EDMaterialColors.Option.MaterialPrimary)
                EDCheckBox {
                    anchors.centerIn: parent
                    checked: visible
                    visible: {
                        if (option === EDMaterialColors.Option.MaterialAccent) {
                            return edColor(modelData, false) === Material.accent
                        } else if (option === EDMaterialColors.Option.MaterialPrimary) {
                            return edColor(modelData, true) === Material.primary
                        }

                        return false
                    }
                }
            }
            onClicked: {
                if (option === EDMaterialColors.Option.MaterialAccent) {
                    edMaterialAccent = edColor(modelData, false)
                } else if (option === EDMaterialColors.Option.MaterialPrimary) {
                    edMaterialPrimary = edColor(modelData, true)
                }
            }
        }
    }

    ToolButton {
        id: addButton
        icon.source: "qrc:/res/icon/IconAdd.svg"
        onClicked: colorDialog.open()
        property string bgColor: "white"
        Layout.alignment: Qt.AlignHCenter|Qt.AlignVCenter
    }


    ColorDialog {
        id: colorDialog
        selectedColor: addButton.bgColor
        onAccepted: {
            addButton.bgColor = selectedColor
            if (option === EDMaterialColors.Option.MaterialAccent) {
                edMaterialAccent = selectedColor
            } else if (option === EDMaterialColors.Option.MaterialPrimary) {
                edMaterialPrimary = selectedColor
            } else {
                console.warn("Unknow option type of Material. The color is:", selectedColor)
            }
        }
    }

    Component.onCompleted: colorRepeater.model = colorModel()

    function edColor(color, isPrimary) {
        if (edMaterialTheme === Material.Dark) {
            if (isPrimary) {
                return Material.color(color)
            } else {
                return Material.color(color, Material.Shade200)
            }
        } else {
            return Material.color(color)
        }
    }

    function colorModel() {
        var colors = []
        colors.push(Material.Red)
        colors.push(Material.Pink)
        colors.push(Material.Purple)
        colors.push(Material.DeepPurple)
        colors.push(Material.Indigo)
        colors.push(Material.Blue)
        colors.push(Material.LightBlue)
        colors.push(Material.Cyan)
        colors.push(Material.Teal)
        colors.push(Material.Green)
        colors.push(Material.LightGreen)
        colors.push(Material.Lime)
        colors.push(Material.Yellow)
        colors.push(Material.Amber)
        colors.push(Material.Orange)
        colors.push(Material.DeepOrange)
        colors.push(Material.Brown)
        colors.push(Material.Grey)
        colors.push(Material.BlueGrey)

       return colors
    }
}
