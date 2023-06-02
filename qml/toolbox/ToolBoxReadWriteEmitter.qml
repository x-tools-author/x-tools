import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import SAK.Custom
import "../common"

Item {
    id: root

    property EDEmitterTool emitterTool: null

    QtObject {
        id: settingKeys
        readonly property string items: groupName + "/emitter/items"
    }

    ToolBoxCommonTableView {
        anchors.fill: parent
        tableMode: emitterTool ? emitterTool.tableModel : null
        tableHeaders: emitterTool ? emitterTool.headers : []
        fillWidthColumns: [11]
        showColumns: [0, 1, 11]
        tabelModelTool: root.emitterTool
        itemsKey: settingKeys.items
        onInvokeAppend: {
            var parameters = emitterTool.itemContext(-1)
            editorPopup.setParameters(parameters)
            editorPopup.index = -1
            editorPopup.open()
        }
        onInvokeEdit: {
            var parameters = emitterTool.itemContext(currentRowIndex)
            editorPopup.setParameters(parameters)
            editorPopup.index = currentRowIndex
            editorPopup.open()
        }
        onItemsChanged: updateSettings()
    }

    ToolBoxReadWriteEmitterPopup {
        id: editorPopup
        onRejected: close()
        onAccepted: {
            close()
            var parameters = getParameters()
            emitterTool.addItem(JSON.stringify(parameters), index)

            updateSettings()
        }

        property int index: -1
    }

//    Component.onCompleted: {
//        var hexString = sakSettings.value(settingKeys.items)
//        var jsonString = sakInterface.hexString2String(hexString);
//        var jsonArray = JSON.parse(jsonString)
//        if (jsonArray) {
//            for (var i = 0; i < jsonArray.length; i++) {
//                var item = jsonArray[i]
//                emitterTool.addItem(JSON.stringify(item), -1)
//            }
//        }
//    }

    function updateSettings() {
        sakSettings.edRemove(settingKeys.group, settingKeys.array)
        var items = emitterTool.itemsContext()
        if (items.length) {
            var itemStringList = []
            for (var i = 0; i < items.length; i++) {
                var item = items[i]
                var itemString = JSON.stringify(item)
                var itemStringHex = sakInterface.string2hexString(itemString)
                itemStringList.push(itemStringHex)
            }

            sakSettings.edSetArrayValues(settingKeys.group,
                                        settingKeys.array,
                                        settingKeys.key,
                                        itemStringList)
        }
    }
}
