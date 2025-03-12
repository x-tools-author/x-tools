import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

Item {
    id: root

    property alias controllerSource: controllerLoader.source
    property Device device: null

    CommonSeparator {
        edge: Qt.RightEdge
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        Loader {
            id: controllerLoader
            Layout.fillWidth: true
            onLoaded: {
                if (item) {
                    item.device = Qt.binding(function () {
                        return root.device
                    })
                }
            }
        }
        Item {
            height: 1
            Layout.fillWidth: true
        }
        RowLayout {
            Layout.fillWidth: true
            ControlsButton {
                text: qsTr("Close")
                enabled: isWorking
                Layout.fillWidth: true
                onClicked: {
                    if (device) {
                        device.closeDevice()
                    }
                }
            }
            ControlsButton {
                text: qsTr("Open")
                enabled: !isWorking
                Layout.fillWidth: true
                onClicked: {
                    if (device && controllerLoader.item) {
                        controllerLoader.item.aboutToOpen()
                        controllerLoader.item.setDeviceParameters()
                        device.openDevice()
                    }
                }
            }
        }
        Label {
            Layout.fillHeight: true
        }
    }
}
