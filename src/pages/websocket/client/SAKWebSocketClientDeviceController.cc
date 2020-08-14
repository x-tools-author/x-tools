/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKWebSocketClientDeviceController.hh"
#include "ui_SAKWebSocketClientDeviceController.h"
SAKWebSocketClientDeviceController::SAKWebSocketClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,ui(new Ui::SAKWebSocketClientDeviceController)
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

QVariant SAKWebSocketClientDeviceController::parameters()
{
    return QVariant::fromValue(true);
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
