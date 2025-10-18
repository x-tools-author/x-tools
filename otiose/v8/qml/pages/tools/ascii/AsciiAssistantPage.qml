import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../../controls"
import xTools

Item {
    id: root

    property var widths: [100, 100, 100, 100, 400]

    AsciiAssistant {
        id: asciiAssistant
    }

    TextField {
        id: searchField
        anchors.topMargin: 4
        width: tableView.width
        anchors.top: parent.top
        height: Material.buttonHeight
        placeholderText: qsTr("Search...")
        anchors.horizontalCenter: parent.horizontalCenter
        onTextEdited: {
            asciiAssistant.filter(searchField.text);
            tableView.contentX = 0;
            tableView.contentY = 0;
        }
        Keys.onReturnPressed: {
            doNotUsedButton.forceActiveFocus();
            asciiAssistant.filter(searchField.text);
        }
    }
    Button {
        id: doNotUsedButton
        visible: false
    }

    Row {
        id: headerRow
        width: tableView.width
        height: Material.buttonHeight
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: searchField.bottom
        anchors.topMargin: 4
        Repeater {
            model: [qsTr("Bin"), qsTr("Oct"), qsTr("Dec"), qsTr("Hex"), qsTr("Description")]
            TextField {
                width: widths[index]
                height: parent.height
                font.bold: true
                text: modelData
                readOnly: true
                verticalAlignment: Text.AlignVCenter
                Keys.onReturnPressed: doNotUsedButton.forceActiveFocus()
                horizontalAlignment: {
                    if (index === 4) {
                        return Text.AlignLeft;
                    } else {
                        return Text.AlignHCenter;
                    }
                }
            }
        }
    }
    TableView {
        id: tableView
        width: 800
        clip: true
        model: asciiAssistant.model()
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: headerRow.bottom
        anchors.bottom: parent.bottom
        columnWidthProvider: function (column) {
            return widths[column];
        }
        rowHeightProvider: function (row) {
            return Material.buttonHeight;
        }
        delegate: TextField {
            text: display
            width: widths[column]
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            Keys.onReturnPressed: doNotUsedButton.forceActiveFocus()
            horizontalAlignment: {
                if (column === 4) {
                    return Text.AlignLeft;
                } else {
                    return Text.AlignHCenter;
                }
            }
        }
    }
}
