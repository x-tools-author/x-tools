import QtQuick
import QtQuick.Controls

CheckBox {
    id: root

    property string settingKey: ""

    onClicked: {
        if (settingKey) {
            sakSettings.setValue(settingKey, root.checked)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var ret = sakSettings.value(settingKey)
            root.checked = ret === edTrue ? true : false
        }
    }
}

