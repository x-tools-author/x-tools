import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import CUAV.Link 1.0

GroupBox {
    title: qsTr("用户信息管理")

    property var userManager: CUAVLinkUserManager

    ColumnLayout {
        anchors.fill: parent
        Layout.alignment: Qt.AlignVCenter

        Row {
            spacing: 10
            Label {
                text: qsTr("修改用户昵称")
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: userNickname
                text: "Qter"
                placeholderText: qsTr("请输入新昵称")
            }

            Button {
                text: qsTr("确认修改")
                onClicked: {
                    userManager.modifyNickname(userNickname.text)
                    userNickname.clear()
                }
            }
        }

        Row {
            spacing: 10
            Label {
                text: qsTr("原密码")
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField {
                id: oldPassword
                text: "12345678"
                placeholderText: qsTr("请输入原密码")                
            }
            Label {
                text: qsTr("新密码")
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField {
                id: newPassword
                text: "87654321"
                placeholderText: qsTr("请输入新密码")
            }
            Label {
                text: qsTr("确认密码")
                verticalAlignment: Qt.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            TextField {
                text: "87654321"
                placeholderText: qsTr("请再次输入新密码")
            }
            Button {
                text: qsTr("确认修改")
                onClicked: {
                    userManager.changeUserPassword(oldPassword.text, newPassword.text)
                }
            }
        }
    }
}

