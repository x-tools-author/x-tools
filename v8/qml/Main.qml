import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: xMainWindow
    width: 1280
    height: 680
    visible: true
    title: "xTools"
    opacity: xMagic ? 0.3 : 1.0

    menuBar: MainHeader {}

    RowLayout {
        anchors.fill: parent
        MainContextNav {
            id: nav
            Layout.fillHeight: true
        }
        MainContextPages {
            id: pages
            currentIndex: nav.navIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
