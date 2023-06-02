import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SAK.Custom
import "../common"

SAKGroupBox {
    id: root
    title: qsTr("Highlighter")
    Layout.fillWidth: true

    QtObject {
        id: settingKeys
        readonly property string highlighterArrayName: "highlighter"
        readonly property string highlighterKeyWord: "highlighterKeyWord"
    }

    EDHighlighter { id: highlighter }

    GridLayout {
        anchors.fill: parent
        columns: 2
        SAKTextField {
            id: keyWordTextField
            placeholderText: qsTr("Key word")
            Layout.fillWidth: true
            Keys.onPressed: function func(event) {
                if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                    appendKeyWord()
                }
            }
        }
        SAKButton {
            text: qsTr("Append")
            onClicked: appendKeyWord()
        }
        SAKLabel {
            text: qsTr("Double click to delete key word")
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
        Flow {
            id: keyWordFlow
            spacing: 4
            Layout.fillWidth: true
            Repeater {
                model: ListModel { id: keyWordListModel }
                SAKLabel {
                    text: keyWord
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            highlighter.removeKeyWord(keyWord)
                            settingPop(index)
                            keyWordListModel.remove(index)
                        }
                    }
                }
            }

        }
    }

    Component.onCompleted: {
        var keyWords = sakSettings.sakArrayValues(groupName, settingKeys.highlighterArrayName, settingKeys.highlighterKeyWord)
        for (var i = 0; i < keyWords.length; i++) {
            keyWordListModel.append({keyWord: keyWords[i]})
        }

        highlighter.setKeyWords(keyWords)
    }

    function setDocument(doc) {
        highlighter.setDoc(doc)
    }

    function isKeyWordExist(keyWord) {
        for (var i = 0; i < keyWordListModel.count; i++) {
            if (keyWordListModel.get(i).keyWord === keyWord) {
                return true
            }
        }

        return false
    }

    function keyWords() {
        var kws = []
        for (var i = 0; i < keyWordListModel.count; i++) {
            kws.push(keyWordListModel.get(i).keyWord)
        }
        return kws
    }

    function setKeyWords() {
        var kws = keyWords()
        highlighter.setKeyWords(kws)
        sakSettings.edSetArrayValues(groupName, settingKeys.highlighterArrayName, settingKeys.highlighterKeyWord, kws)
    }

    function settingPop(index) {
        var kws = keyWords()
        kws.splice(index, 1)
        sakSettings.edRemove(groupName, settingKeys.highlighterKeyWord)
        if (kws.length) {
            sakSettings.edSetArrayValues(groupName, settingKeys.highlighterKeyWord, kws)
        }
    }

    function appendKeyWord() {
        if (!isKeyWordExist(keyWordTextField.text.trim())) {
            keyWordListModel.append({keyWord: keyWordTextField.text.trim()})
            setKeyWords()
        }
    }
}
