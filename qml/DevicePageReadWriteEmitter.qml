import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import ED.EasyDebug
import "common"

Item {
    id: root

    property EDEmitterTool emitterTool: null

    QtObject {
        id: settingKeys
        readonly property string group: groupName + "Emitter"
        readonly property string array: "emitter"
        readonly property string key: "itemData"
    }

    DevicePageCommonTableView {
        anchors.fill: parent
        tableMode: emitterTool ? emitterTool.tableModel : null
        tableHeaders: emitterTool ? emitterTool.headers : []
        fillWidthColumns: [11]
        showColumns: [0, 1, 11]
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

    DevicePageReadWriteEmitterPopup {
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

    Component.onCompleted: {
        var hexStringList = edSettings.edArrayValues(settingKeys.group,
                                                     settingKeys.array,
                                                     settingKeys.key)
        if (hexStringList.length) {
            var jsonArray = []
            for (var i = 0; i < hexStringList.length; i++) {
                var hexString = hexStringList[i];
                var jsonString = edInterface.hexString2String(hexString)
                jsonArray.push(JSON.parse(jsonString))
            }
            if (jsonArray.length) {
                edInterface.jsonArray2TableModel(emitterTool.tableModel, jsonArray)
            }
        }
    }

    function updateSettings() {
        edSettings.edRemove(settingKeys.group, settingKeys.array)
        var items = emitterTool.itemsContext()
        if (items.length) {
            var itemStringList = []
            for (var i = 0; i < items.length; i++) {
                var item = items[i]
                var itemString = JSON.stringify(item)
                var itemStringHex = edInterface.string2hexString(itemString)
                itemStringList.push(itemStringHex)
            }

            edSettings.edSetArrayValues(settingKeys.group,
                                        settingKeys.array,
                                        settingKeys.key,
                                        itemStringList)
        }
    }
}
