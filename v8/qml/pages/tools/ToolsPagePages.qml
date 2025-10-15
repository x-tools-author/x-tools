import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../controls"

StackLayout {
    id: root

    property alias model: pageRepeater.model

    Repeater {
        id: pageRepeater
        Loader {
            source: modelData
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
