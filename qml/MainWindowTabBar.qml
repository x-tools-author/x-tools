import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    id: root
    color: "#101010"
    height: 28

    property var tabButtons: []
    property int currentIndex: 0

    RowLayout {
        spacing: 10
        anchors{left: parent.left;top: parent.top;bottom: parent.bottom;leftMargin: 10}

        Repeater {
            id: tabButtonsRepeater
            model: tabButtons

            Label {
                height: parent.height
                text: modelData
                color: (currentIndex == index) ?  "#01D766" : "#FFFFFF"
                font.pixelSize: 12
                verticalAlignment: Qt.AlignVCenter
                Layout.fillHeight: true

                Rectangle {
                    anchors{left: parent.left;right: parent.right;bottom: parent.bottom}
                    height: 2
                    color: "#01D766"
                    visible: currentIndex == index
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    enabled: !retct.visible
                    onClicked: {
                        currentIndex = index
                        console.info(modelData)
                    }
                }

                // 间隔符
                Rectangle {
                    id: retct
                    width: 2
                    height: root.height*0.6
                    color: "#01D766"
                    visible: modelData === " "
                    anchors.centerIn: parent
                }
            }
        }
    }

    function push(_title){
        tabButtons.push(_title)
        tabButtonsRepeater.model = tabButtons
    }
}
