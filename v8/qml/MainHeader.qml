import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

MenuBar {
    id: menuBar
    Menu {
        title: qsTr("File")
        MenuItem {
            text: qsTr("New")
        }
        MenuItem {
            text: qsTr("Open")
        }
        MenuItem {
            text: qsTr("Save")
        }
        MenuItem {
            text: qsTr("Save As")
        }
        MenuItem {
            text: qsTr("Exit")
            onTriggered: Qt.quit()
        }
    }
    Menu {
        title: qsTr("Tools")
        MenuItem {
            text: qsTr("About Qt")
        }
        MenuItem {
            text: qsTr("About xTools")
        }
    }
    Menu {
        title: qsTr("Help")
        MenuItem {
            text: qsTr("About Qt")
        }
        MenuItem {
            text: qsTr("About xTools")
        }
    }
}
