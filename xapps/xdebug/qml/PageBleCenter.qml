import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: BleCentral {}
    controllerSource: "qrc:/qml/ControllerBleCenter.qml"
    settingsGroup: "BleCenter"
}
