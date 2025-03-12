import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root

    property int column0width: 84
    property int column1width: 168
    property int column2width: 84
    property int column3width: 84
    property int rowHeight: 40

    RowLayout {
        id: headerRowLayout
        spacing: 0
        width: tableView.width
        Repeater {
            model: {
                var tmp = []
                tmp.push(qsTr("Number"))
                tmp.push(qsTr("IP Address"))
                tmp.push(qsTr("Status"))
                tmp.push(qsTr("Compare"))
                tmp.push(qsTr("Host Nmae"))
                return tmp
            }
            MainWindowTableViewCell {
                text: modelData
                leftBorderVisible: index === 0
                topBorderVisible: true
                enableBackground: true
                Layout.minimumHeight: rowHeight
                Layout.minimumWidth: {
                    return columnWidth(index)
                }
            }
        }
    }

    TableView {
        id: tableView
        clip: true
        reuseItems: false
        contentWidth: -1
        rowSpacing: 0
        columnSpacing: 0
        boundsBehavior: Flickable.StopAtBounds
        model: xApp.proxyModel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerRowLayout.bottom
        anchors.bottom: parent.bottom
        columnWidthProvider: function (column) {
            return root.columnWidth(column)
        }
        rowHeightProvider: function (row) {
            return root.rowHeight
        }
        delegate: MainWindowTableViewCell {
            text: display
            leftBorderVisible: column === 0
            enableTextColor: column === 2
            busyIndicatorVisible: column === 0
            Component.onCompleted: {
                if (column === 0) {
                    isPinging = Qt.binding(function () {
                        return edit
                    })
                } else if (column === 2) {
                    rawStatus = Qt.binding(function () {
                        return edit
                    })
                }
            }
        }
        Component.onCompleted: {
            xPingTableViewRow = Qt.binding(function () {
                return tableView.rows
            })
        }
    }

    Label {
        id: infoLabel
        text: String("%1/%2").arg(xPingTableViewRow).arg(xApp.total)
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: xPingCentralRectangle.border.width
        anchors.rightMargin: xPingCentralRectangle.border.width
    }

    Item {
        id: noDataItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerRowLayout.bottom
        anchors.bottom: parent.bottom
        visible: tableView.rows === 0
        Image {
            source: "qrc:/Resources/Icons/NoData.svg"
            width: parent.width * 0.6
            height: parent.height * 0.6
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            MultiEffect {
                source: parent
                anchors.fill: parent
                colorization: 1.0
                colorizationColor: xPingPrimaryColor
            }
        }
        Rectangle {
            width: 2
            height: parent.height
            color: xPingPrimaryColor
        }
        Rectangle {
            width: 2
            height: parent.height
            color: xPingPrimaryColor
            anchors.right: parent.right
        }
        Rectangle {
            width: parent.width
            height: 2
            color: xPingPrimaryColor
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
        Label {
            anchors.centerIn: parent
            text: qsTr("No Data Yet")
            anchors.verticalCenterOffset: -60
        }
    }

    function columnWidth(index) {
        if (index === 0) {
            return root.column0width
        } else if (index === 1) {
            return root.column1width
        } else if (index === 2) {
            return root.column2width
        } else if (index === 3) {
            return root.column3width
        } else {
            var tmp = root.width
            tmp = tmp - root.column0width
            tmp = tmp - root.column1width
            tmp = tmp - root.column2width
            tmp = tmp - root.column3width
            return tmp
        }
    }

    function resetTableViewY() {
        tableView.contentY = 0
    }
}
