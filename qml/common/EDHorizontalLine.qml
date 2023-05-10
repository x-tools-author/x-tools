import QtQuick

Rectangle {
    id: root
    color: "gray"
    opacity: 0.5
    height: 1

    property bool autoScale: true

    onParentChanged: {
        if (parent && autoScale) {
            anchors.left = Qt.binding(function func(){ return parent.left })
            anchors.right = Qt.binding(function func(){ return parent.right })
        }
    }
}
