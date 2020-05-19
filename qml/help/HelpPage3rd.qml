import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0
import "qrc:/qml/base"

HelpPagePanel {
    id: root

    property SAKHelp3rdController _3rdController: helpManager.trdController
    property var ursl: [
        "libusb", "https://github.com/libusb/libusb",
        "hidapi", "https://github.com/libusb/hidapi",
    ]

    GridLayout {
        width: (root.width - root.leftPadding - root.rightPadding)
        columns: 2

        Repeater {
            model: ursl
            SAKLabel {
                text: modelData
                color: mouseArea.containsMouse ? "#0000ff" : "#ffffff"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    visible: index%2

                    onClicked: {
                        _3rdController.openUrl(modelData)
                    }
                }
            }
        }
    }
}
