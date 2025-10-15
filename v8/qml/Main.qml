import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import "common"

ApplicationWindow {
    id: xMainWindow
    width: 1280
    height: 680
    visible: true
    title: "xTools"
    opacity: xMagic ? 0.3 : 1.0
    menuBar: MainHeader {}

    property string xDisabledText: qsTr("Disable")
    property string xNoneText: qsTr("None")
    property alias xKeysObj: settingKeys
    readonly property int theme: xApp.settingsValue(settingKeys.materialTheme, Material.Dark)
    readonly property int primary: xApp.settingsValue(settingKeys.materialPrimary, Material.Indigo)
    property int xMaterialTheme: theme ? theme : Material.Dark
    property int xMaterialPrimary: primary ? primary : Material.Indigo
    property bool isWindowed: visibility === Window.Windowed
    readonly property string xTrue: "true"
    readonly property string xFalse: "false"
    property int xColorShade: xMaterialTheme === Material.Dark ? Material.Shade200 : Material.Shade500
    property int xPreferredWidth: 800

    Material.theme: xMaterialTheme
    Material.accent: Material.color(xMaterialPrimary, xColorShade)
    Material.primary: Material.color(xMaterialPrimary, xColorShade)

    QtObject {
        id: settingKeys
        readonly property string materialTheme: "materialTheme"
        readonly property string materialPrimary: "materialPrimary"
        readonly property string pageIndex: "pageIndex"
    }

    Item {
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent
            MainContextNav {
                id: nav
                Layout.fillHeight: true
                onNavIndexChanged: {
                    xApp.setSettingsValue(settingKeys.pageIndex, nav.navIndex);
                }
            }
            MainContextPages {
                id: pages
                currentIndex: nav.navIndex
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
        CommonSeparator {
            edge: Qt.TopEdge
        }
    }

    Component.onCompleted: {
        var index = xApp.settingsValue(settingKeys.pageIndex, 0);
        nav.navIndex = index;
        nav.setChecked(index);
    }
}
