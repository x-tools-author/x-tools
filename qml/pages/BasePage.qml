import QtQuick
import QtQuick.Layouts

import "../common"

SAKPane {
    id: root

    Item {
        id: controllerArea
        width: 250
        height: root.height
        Loader {
            anchors.fill: parent
        }
    }
    Item {
        id: outoutArea
    }
    Item {
        id: inputArea
    }
}
