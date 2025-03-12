import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

BasePage {
    id: root
    device: Serialport {}
    controllerSource: "qrc:/qml/ControllerSerialPort.qml"
    settingsGroup: "SerialPort"
}
