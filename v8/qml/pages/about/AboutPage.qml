import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../controls"

Pane {
    id: root

    Column {
        ControlLabel {
            text: qsTr("Application Information")
            font.bold: true
        }
        Label {
            text: " "
        }
        GridLayout {
            columns: 3
            columnSpacing: 8
            ControlLabel {
                text: qsTr("Version")
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "8.0.0"
            }

            ControlLabel {
                text: "Commit Hash"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "0000000000000000"
            }

            ControlLabel {
                text: "Build Date"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "2025-08-06 10:00:00"
            }

            ControlLabel {
                text: "Github respository"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "2025-08-06 10:00:00"
            }

            ControlLabel {
                text: "Gitee respository"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "2025-08-06 10:00:00"
            }

            ControlLabel {
                text: "Copyright"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "Copyright (C) 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved."
            }
        }

        Label {
            text: " "
        }
        ControlLabel {
            text: qsTr("The third-party open source libraries used in this application")
            font.bold: true
        }
        Label {
            text: " "
        }
        Column {
            Repeater {
                model: {
                    var tmp = [];
                    tmp.push(["lua", "https://github.com/lua/lua"]);
                    tmp.push(["glog", "https://github.com/google/glog"]);
                    tmp.push(["zint", "https://github.com/zint/zint"]);
                    tmp.push(["QXlsx", "https://github.com/QtExcel/QXlsx"]);
                    tmp.push(["libiconv", "https://ftp.gnu.org/pub/gnu/libiconv"]);
                    tmp.push(["libqrencode", "https://github.com/fukuchi/libqrencode"]);
                    tmp.push(["qmdnsengine", "https://github.com/nitroshare/qmdnsengine"]);
                    tmp.push(["SingleApplication", "https://github.com/itay-grudev/SingleApplication"]);
                    tmp.push(["hidapi", "https://github.com/libusb/hidapi"]);
                    return tmp;
                }

                ControlLabel {
                    text: modelData[0] + " - " + modelData[1]
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            Qt.openUrlExternally(modelData[1]);
                        }
                    }
                }
            }
        }
    }
}
