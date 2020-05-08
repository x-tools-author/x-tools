import QtQuick 2.12
import QtQuick.Controls 2.12

import SAK.CustomType 1.0

import "qrc:/qml/component"

Page {
    id: root
    background: Rectangle{color: "#161616"}

    property string sakIcon: ""
    property string sakTitle: ""
    property SAKDebugger sakdebugger: null

    header: SAKTitle{
        icon: sakIcon
        title: sakTitle
    }
}
