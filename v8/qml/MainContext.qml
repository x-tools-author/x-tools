import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: xMainWindow
    width: 1280
    height: 680

    RowLayout {
        anchors.fill: parent
        MainContextNav {
            Layout.fillHeight: true
        }
        MainContextPages {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
