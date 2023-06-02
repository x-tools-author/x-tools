import QtQuick
import QtQuick.Controls

TextField {
    id: root

    property string settingKey: ""

    onTextEdited: {
        if (settingKey) {
            sakSettings.setValue(settingKey, root.text)
        }
    }
    Component.onCompleted: {
        if (settingKey) {
            var txt = sakSettings.value(settingKey)
            if (txt) {
                root.text = txt
            }
        }
    }
}

