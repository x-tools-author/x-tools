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
                text: xApp.gitCommit()
            }

            ControlLabel {
                text: "Build Date"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: xApp.buildDateTime()
            }

            ControlLabel {
                text: "Github respository"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                id: githubLabel
                text: "https://github.com/x-tools-author/x-tools"
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Qt.openUrlExternally(githubLabel.text);
                    }
                }
            }

            ControlLabel {
                text: "Gitee respository"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                id: giteeLabel
                text: "https://gitee.com/x-tools-author/x-tools"
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Qt.openUrlExternally(giteeLabel.text);
                    }
                }
            }

            ControlLabel {
                text: qsTr("Online Documentation")
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                id: onlineLabel
                text: "https://x-tools-author.github.io/x-tools/"
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Qt.openUrlExternally(onlineLabel.text);
                    }
                }
            }

            ControlLabel {
                text: "Copyright"
            }
            ControlLabel {
                text: ":"
            }
            ControlLabel {
                text: "Copyright © 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved."
            }
        }

        Label {
            text: " "
        }
        ControlLabel {
            id: libsTitleLabel
            text: qsTr("The third-party open source libraries used in this application")
            font.bold: true
        }
        Label {
            text: " "
        }
        Column {
            id: libsColumn

            property int maxLabelLen: 0

            Repeater {
                model: {
                    var tmp = [];
                    tmp.push(["lua", "https://github.com/lua/lua"]);
                    tmp.push(["glog", "https://github.com/google/glog"]);
                    tmp.push(["zint", "https://github.com/zint/zint"]);
                    tmp.push(["QXlsx", "https://github.com/QtExcel/QXlsx"]);
                    tmp.push(["hidapi", "https://github.com/libusb/hidapi"]);
                    tmp.push(["libiconv", "https://ftp.gnu.org/pub/gnu/libiconv"]);
                    tmp.push(["libqrencode", "https://github.com/fukuchi/libqrencode"]);
                    tmp.push(["qmdnsengine", "https://github.com/nitroshare/qmdnsengine"]);
                    tmp.push(["SingleApplication", "https://github.com/itay-grudev/SingleApplication"]);
                    return tmp;
                }

                MouseArea {
                    width: libRow.width
                    height: libsTitleLabel.height
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Qt.openUrlExternally(modelData[1]);
                    }
                    RowLayout {
                        id: libRow
                        ControlLabel {
                            text: modelData[0]
                            Component.onCompleted: {
                                if (width > libsColumn.maxLabelLen) {
                                    libsColumn.maxLabelLen = width;
                                }

                                Layout.minimumWidth = Qt.binding(function () {
                                    return libsColumn.maxLabelLen;
                                });
                            }
                        }
                        ControlLabel {
                            text: ":"
                        }
                        ControlLabel {
                            text: modelData[1]
                        }
                    }
                }
            }
        }
    }
}
