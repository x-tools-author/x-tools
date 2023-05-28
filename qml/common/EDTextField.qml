import QtQuick
import QtQuick.Controls

TextField {
    id: root

    property string settingKey: ""

    onTextEdited: {
        if (settingKey) {
            sakSettings.edSetValue(settingKey, root.text)
        }
    }
    Component.onCompleted: {
        if (settingKey) {
            var txt = sakSettings.edValue(settingKey)
            if (txt) {
                root.text = txt
            }
        }
    }
}

