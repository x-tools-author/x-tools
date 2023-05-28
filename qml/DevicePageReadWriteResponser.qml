import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ED.EasyDebug
import "common"

Item {
    id: root

    property EDResponserTool responserTool: null

    QtObject {
        id: settingKeys
        readonly property string group: groupName + "Responser"
        readonly property string array: "responser"
        readonly property string key: "itemData"
    }

    DevicePageCommonTableView {
        id: tableView
        anchors.fill: parent
        tableMode: responserTool ? responserTool.tableModel : null
        tableHeaders: responserTool ? responserTool.headers : []
        fillWidthColumns: [11, 21]
        showColumns: [0, 1, 2, 11, 21]
        onInvokeAppend: {
            var parameters = responserTool.itemContext(-1)
            editorPopup.setParameters(parameters)
            editorPopup.index = -1
            editorPopup.open()
        }
        onInvokeEdit: {
            var parameters = responserTool.itemContext(currentRowIndex)
            editorPopup.setParameters(parameters)
            editorPopup.index = currentRowIndex
            editorPopup.open()
        }
        onItemsChanged: updateSettings()
    }

    DevicePageReadWriteResponserPopup {
        id: editorPopup
        onRejected: close()
        onAccepted: {
            close()
            var parameters = getParameters()
            responserTool.addItem(JSON.stringify(parameters), index)

            updateSettings()
        }

        property int index: -1
    }

    Component.onCompleted: {
        var hexStringList = sakSettings.edArrayValues(settingKeys.group,
                                                     settingKeys.array,
                                                     settingKeys.key)
        if (hexStringList.length) {
            var jsonArray = []
            for (var i = 0; i < hexStringList.length; i++) {
                var hexString = hexStringList[i];
                var jsonString = sakInterface.hexString2String(hexString)
                jsonArray.push(JSON.parse(jsonString))
            }
            if (jsonArray.length) {
                sakInterface.jsonArray2TableModel(responserTool.tableModel, jsonArray)
            }
        }
    }

    function updateSettings() {
        sakSettings.edRemove(settingKeys.group, settingKeys.array)
        var items = responserTool.itemsContext()
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
