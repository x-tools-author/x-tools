import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../controls"
import "../../common"

Pane {
    id: root
    ScrollView {
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        Column {
            CommonInfoGroupBox {
                width: 800
                title: qsTr("Application Information")
                model: {
                    var tmp = [];
                    tmp.push([qsTr("Version"), "8.0.0", false]);
                    tmp.push(["Commit Hash", xApp.gitCommit(), false]);
                    tmp.push(["Build Date", xApp.buildDateTime(), false]);
                    tmp.push([qsTr("Github Repository"), "https://github.com/x-tools-author/x-tools", true]);
                    tmp.push([qsTr("Gitee Repository"), "https://gitee.com/x-tools-author/x-tools", true]);
                    tmp.push([qsTr("Online Documentation"), "https://x-tools-author.github.io/x-tools/", true]);
                    tmp.push(["Copyright", qsTr("Copyright © 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved."), false]);
                    return tmp;
                }
            }

            Label {
                text: " "
            }

            CommonInfoGroupBox {
                width: 800
                title: qsTr("The third-party open source libraries used in this application")
                model: {
                    var tmp = [];
                    tmp.push(["lua", "https://github.com/lua/lua", true]);
                    tmp.push(["glog", "https://github.com/google/glog", true]);
                    tmp.push(["zint", "https://github.com/zint/zint", true]);
                    tmp.push(["QXlsx", "https://github.com/QtExcel/QXlsx", true]);
                    tmp.push(["hidapi", "https://github.com/libusb/hidapi", true]);
                    tmp.push(["libiconv", "https://ftp.gnu.org/pub/gnu/libiconv", true]);
                    tmp.push(["libqrencode", "https://github.com/fukuchi/libqrencode", true]);
                    tmp.push(["qmdnsengine", "https://github.com/nitroshare/qmdnsengine", true]);
                    tmp.push(["SingleApplication", "https://github.com/itay-grudev/SingleApplication", true]);
                    return tmp;
                }
            }
        }
    }
}
