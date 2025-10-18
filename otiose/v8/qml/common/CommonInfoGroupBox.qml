import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../controls"

GroupBox {
    id: root

    property alias model: infoRepeater.model
    property int maxLabelLen: 0

    Column {
        id: libsColumn
        anchors.fill: parent

        Repeater {
            id: infoRepeater
            MouseArea {
                enabled: modelData[2] ? modelData[2] : false
                width: parent.width
                height: infoRowLayout.height
                cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                onClicked: {
                    Qt.openUrlExternally(modelData[1]);
                }

                RowLayout {
                    id: infoRowLayout
                    anchors.left: parent.left
                    anchors.right: parent.right
                    ControlLabel {
                        text: modelData[0]
                        Component.onCompleted: {
                            if (width > root.maxLabelLen) {
                                root.maxLabelLen = width;
                            }

                            Layout.minimumWidth = Qt.binding(function () {
                                return root.maxLabelLen;
                            });
                        }
                    }
                    ControlLabel {
                        text: ":"
                    }
                    ControlLabel {
                        text: modelData[1]
                        Layout.fillWidth: true
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }
        }
    }
}
