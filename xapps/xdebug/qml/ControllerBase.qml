import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import xTools.xDebug

ColumnLayout {
    id: root

    property Device device: null

    function save() {
        console.error("Please implement save() method...")
        return {}
    }

    function aboutToOpen() {
        console.error("Please implement aboutToOpen() method...")
    }

    function setDeviceParameters() {
        if (device) {
            var parameters = save()
            device.load(parameters)
        }
    }
}
