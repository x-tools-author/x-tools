import QtQuick
import QtQuick.Controls

CheckBox {
    id: root

    property string settingKey: ""

    onClicked: {
        if (settingKey) {
            edSettings.edSetValue(settingKey, root.checked)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var ret = edSettings.edValue(settingKey)
            root.checked = ret === edTrue ? true : false
        }
    }
}

