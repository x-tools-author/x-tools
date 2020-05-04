import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/component"

Page {
    id: root
    background: Rectangle{color: "#161616"}

    property string sakIcon: ""
    property string sakTitle: ""

    header: SAKTitle{
        icon: sakIcon
        title: sakTitle
    }
}
