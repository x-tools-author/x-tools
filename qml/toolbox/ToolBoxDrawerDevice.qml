import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "../common"

SAKPane {
    id: root
    property alias txMasker: maskerGroupBox.txMasker
    property alias rxMasker: maskerGroupBox.rxMasker
    property alias analyzer: analyzerGroupBox.analyzer
    ScrollView {
        anchors.fill: parent
        ColumnLayout {
            width: root.width
            ToolBoxDrawerDeviceMasker {
                id: maskerGroupBox
                Layout.fillWidth: true
            }
            ToolBoxDrawerDeviceAnalyzer {
                id: analyzerGroupBox
                Layout.fillWidth: true
            }
        }
    }

}
