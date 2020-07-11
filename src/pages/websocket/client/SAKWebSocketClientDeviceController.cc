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
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKWebSocketClientDeviceController.hh"
#include "ui_SAKWebSocketClientDeviceController.h"
SAKWebSocketClientDeviceController::SAKWebSocketClientDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKWebSocketClientDeviceController)
{
    ui->setupUi(this);
    serverAddressLineEdit = ui->serverHostLineEdit;
    sendingTypeComboBox = ui->sendingTypeComboBox;
    SAKGlobal::initWebSocketSendingTypeComboBox(sendingTypeComboBox);
}

SAKWebSocketClientDeviceController::~SAKWebSocketClientDeviceController()
{
    delete ui;
}

QString SAKWebSocketClientDeviceController::serverAddress()
{
    uiMutex.lock();
    QString ret = serverAddressLineEdit->text();
    uiMutex.unlock();
    return ret;
}

void SAKWebSocketClientDeviceController::setUiEnable(bool enable)
{
    serverAddressLineEdit->setEnabled(enable);
}

quint32 SAKWebSocketClientDeviceController::sendingType()
{
    uiMutex.lock();
    quint32 ret = sendingTypeComboBox->currentData().toUInt();
    uiMutex.unlock();
    return ret;
}
