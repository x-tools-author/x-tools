import QtQuick

Rectangle {
    id: root
    color: "gray"
    opacity: 0.5
    width: 1

    property bool autoScale: true

    onParentChanged: {
        if (parent && autoScale) {
            anchors.top = Qt.binding(function func(){ return parent.top })
            anchors.bottom = Qt.binding(function func(){ return parent.bottom })
        }
    }
}
