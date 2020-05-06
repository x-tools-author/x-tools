import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "qrc:/qml/base"

DebugPageBlock {
    id: root
    sakIcon: "qrc:/resources/icons/控制.png"
    sakTitle: qsTr("设备控制")

    contentItem: Item{
        Rectangle {
            id: controlPannel
            color: "#00000000"
            anchors {
                fill: parent
                margins: 5
            }
        }
    }

    function createControlPannel(source){
        if (source){
            var component = Qt.createComponent(source);
            if (component && (component.status === Component.Ready)){
                component.createObject(controlPannel);
            }else{
                console.error("Oh, something is error!")
            }
        }
    }
}
