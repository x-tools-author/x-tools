import QtQuick
import QtQuick.Controls

SpinBox {
    id: root
    property string settingKey: ""
    from: 0
    to: 24*60*60*1000
    editable: true

    onValueModified: {
        if (settingKey) {
            sakSettings.sakSetValue(settingKey, root.value)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var v = sakSettings.sakValue(settingKey)
            if (v) {
                root.value = v
            }
        }
    }
}
