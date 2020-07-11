/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKWebSocketServerDeviceController.hh"
#include "ui_SAKWebSocketServerDeviceController.h"
SAKWebSocketServerDeviceController::SAKWebSocketServerDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKWebSocketServerDeviceController)
{
    ui->setupUi(this);

    serverHostComboBox = ui->serverhostComboBox;
    serverPortLineEdit = ui->serverPortLineEdit;
    clientHostComboBox = ui->clientHostComboBox;
    sendingTypeComboBox = ui->sendingTypeComboBox;
    SAKGlobal::initWebSocketSendingTypeComboBox(sendingTypeComboBox);
    /// @brief 获取本机网络地址
    refresh();
}

SAKWebSocketServerDeviceController::~SAKWebSocketServerDeviceController()
{
    delete ui;
}

QString SAKWebSocketServerDeviceController::serverHost()
{
    uiMutex.lock();
    QString host = serverHostComboBox->currentText();
    uiMutex.unlock();
    return host;
}

quint16 SAKWebSocketServerDeviceController::serverPort()
{
    uiMutex.lock();
    quint16 port = static_cast<quint16>(serverPortLineEdit->text().toInt());
    uiMutex.unlock();
    return port;
}

QString SAKWebSocketServerDeviceController::currentClientHost()
{
    uiMutex.lock();
    QStringList host = clientHostComboBox->currentText().split(":");
    QString address = host.first();
    uiMutex.unlock();
    return address;
}

quint16 SAKWebSocketServerDeviceController::currentClientPort()
{
    uiMutex.lock();
    QString port = clientHostComboBox->currentText().split(":").last();
    quint16 portTemp = static_cast<quint16>(port.toInt());
    uiMutex.unlock();
    return portTemp;
}

quint32 SAKWebSocketServerDeviceController::sendingType()
{
    uiMutex.lock();
    quint32 ret = sendingTypeComboBox->currentData().toInt();
    uiMutex.unlock();
    return ret;
}

void SAKWebSocketServerDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(serverHostComboBox);
}

void SAKWebSocketServerDeviceController::setUiEnable(bool enable)
{
    serverHostComboBox->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}

void SAKWebSocketServerDeviceController::addClient(QString host, quint16 port, QWebSocket *socket)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    uiMutex.lock();
    for(int i = 0; i < clientHostComboBox->count(); i++){
        if (clientHostComboBox->itemText(i).compare(item) == 0){
            uiMutex.unlock();
            return;
        }
    }

    clientHostComboBox->addItem(item, QVariant::fromValue(socket));
    uiMutex.unlock();
}

void SAKWebSocketServerDeviceController::removeClient(QWebSocket *socket)
{
    uiMutex.lock();
    for(int i = 0; i < clientHostComboBox->count(); i++){
        if (clientHostComboBox->itemData(i).value<QWebSocket*>() == socket){
            clientHostComboBox->removeItem(i);
            break;
        }
    }
    uiMutex.unlock();
}

void SAKWebSocketServerDeviceController::clearClient()
{
    uiMutex.lock();
    clientHostComboBox->clear();
    uiMutex.unlock();
}
