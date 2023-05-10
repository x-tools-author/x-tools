import QtQuick
import QtQuick.Controls

TextField {
    id: root

    property string settingKey: ""

    onTextEdited: {
        if (settingKey) {
            edSettings.edSetValue(settingKey, root.text)
        }
    }
    Component.onCompleted: {
        if (settingKey) {
            var txt = edSettings.edValue(settingKey)
            if (txt) {
                root.text = txt
            }
        }
    }
}

