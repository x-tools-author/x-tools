import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: xMainWindow
    width: 1280
    height: 680
    visible: true
    title: "xTools"
    opacity: xMagic ? 0.3 : 1.0

    footer: MainFooter {}

    MainContext {
        anchors.fill: parent
    }
}
