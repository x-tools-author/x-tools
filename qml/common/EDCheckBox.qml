import QtQuick
import QtQuick.Controls

CheckBox {
    id: root

    property string settingKey: ""

    onClicked: {
        if (settingKey) {
            sakSettings.edSetValue(settingKey, root.checked)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var ret = sakSettings.edValue(settingKey)
            root.checked = ret === edTrue ? true : false
        }
    }
}

