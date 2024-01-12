import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import QtQuick.Controls.Material

import SAK.Custom
import "common"
import "pages"

Item {
    id: root

    property alias pageIndex: pageStackLayout.currentIndex

    StackLayout {
        id: pageStackLayout
        anchors.fill: parent
        SerialPortPage {
            id: comPage
        }

        /*BleCentralPage {
            id: blePage
        }
        UdpClientPage {
            id: udpClientPage
        }
        UdpServerPage {
            id: udpServerPage
        }
        TcpClientPage {
            id: tcpClientPage
        }
        TcpServerPage {
            id: tcpServerPage
        }
        WebSocketClientPage {
            id: websocketClientPage
        }
        WebSocketServerPage {
            id: websocketServerPage
        }*/
        AboutPage {
            id: aboutPage
        }
        SettingsPage {
            id: settingsPage
        }
    }
}
