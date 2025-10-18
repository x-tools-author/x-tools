import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../controls"

Item {
    id: root

    property var model: {
        var tmp = [];

        tmp.push([qsTr("ASCII Assistant"), "ascii/AsciiAssistantPage.qml"]);
        tmp.push([qsTr("Bar Code Assistant"), "barcode/BarCodeAssistantPage.qml"]);
        tmp.push([qsTr("Base64 Assistant"), "base64/Base64AssistantPage.qml"]);
        tmp.push([qsTr("CRC Assistant"), "crc/CrcAssistantPage.qml"]);
        tmp.push([qsTr("File Checking Assistant"), "filecheck/FileCheckingAssistantPage.qml"]);
        tmp.push([qsTr("File Merging Assistant"), "filemerge/FileMergingAssistantPage.qml"]);
        tmp.push([qsTr("MDNS Assistant"), "mdns/MdnsAssistantPage.qml"]);
        tmp.push([qsTr("Number Assistant"), "number/NumberAssistantPage.qml"]);
        tmp.push([qsTr("Ping Assistant"), "ping/PingAssistantPage.qml"]);
        tmp.push([qsTr("Qr Code Assistant"), "qrcode/QrCodeAssistantPage.qml"]);
        tmp.push([qsTr("String Assistant"), "string/StringAssistantPage.qml"]);

        return tmp;
    }

    RowLayout {
        anchors.fill: parent
        ToolsPageNav {
            id: navLayout
            Layout.fillHeight: true
            model: {
                var tmp = [];
                for (var i = 0; i < root.model.length; i++) {
                    tmp.push(root.model[i][0]);
                }
                return tmp;
            }
        }
        ToolsPagePages {
            id: pagesLayout
            currentIndex: navLayout.navIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: {
                var tmp = [];
                for (var i = 0; i < root.model.length; i++) {
                    tmp.push(root.model[i][1]);
                }
                return tmp;
            }
        }
    }
}
