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
            sakSettings.setValue(settingKey, root.value)
        }
    }

    Component.onCompleted: {
        if (settingKey) {
            var v = sakSettings.value(settingKey)
            if (v) {
                root.value = v
            }
        }
    }
}
