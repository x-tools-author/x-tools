import QtQuick
import QtQuick.Controls

TextField {
    id: root

    property string settingKey: ""

    onTextEdited: {
        if (settingKey) {
            sakSettings.sakSetValue(settingKey, root.text)
        }
    }
    Component.onCompleted: {
        if (settingKey) {
            var txt = sakSettings.sakValue(settingKey)
            if (txt) {
                root.text = txt
            }
        }
    }
}

