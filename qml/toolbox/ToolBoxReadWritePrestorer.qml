import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import SAK.Custom
import "../common"

Item {
    id: root

    property EDPrestorerTool presotrerTool: null

    QtObject {
        id: settingKeys
        readonly property string group: groupName + "Prestore"
        readonly property string array: "item"
        readonly property string key: "itemData"
    }

    ToolBoxCommonTableView {
        anchors.fill: parent
        tableMode: presotrerTool ? presotrerTool.tableModel : null
        tableHeaders: presotrerTool ? presotrerTool.headers : []
        fillWidthColumns: [9]
        showColumns: [0, 9]
        onInvokeAppend: {
            var parameters = presotrerTool.itemContext(-1)
            editorPopup.setParameters(parameters)
            editorPopup.index = -1
            editorPopup.open()
        }
        onInvokeEdit: {
            var parameters = presotrerTool.itemContext(currentRowIndex)
            editorPopup.setParameters(parameters)
            editorPopup.index = currentRowIndex
            editorPopup.open()
        }
        onItemsChanged: updateSettings()
    }

    ToolBoxReadWritePrestorerPopup {
        id: editorPopup
        onRejected: close()
        onAccepted: {
            close()
            var parameters = getParameters()
            presotrerTool.addItem(JSON.stringify(parameters), index)

            updateSettings()
        }

        property int index: -1
    }

    Component.onCompleted: {
        var hexStringList = sakSettings.sakArrayValues(settingKeys.group,
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
                sakInterface.jsonArray2TableModel(presotrerTool.tableModel, jsonArray)
            }
        }
    }

    function updateSettings() {
        sakSettings.edRemove(settingKeys.group, settingKeys.array)
        var items = presotrerTool.itemsContext()
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
