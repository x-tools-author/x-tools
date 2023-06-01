import QtQuick
import QtQuick.Controls

CheckBox {
    id: root

    property string settingKey: ""

    onClicked: {
        if (settingKey) {
            sakSettings.sakSetValue(settingKey, root.checked)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var ret = sakSettings.sakValue(settingKey)
            root.checked = ret === edTrue ? true : false
        }
    }
}

