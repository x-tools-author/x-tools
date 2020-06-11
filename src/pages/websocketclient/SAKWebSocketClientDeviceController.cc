/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
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

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    serverHostLineEdit = ui->serverHostLineEdit;
    serverPortLineEdit = ui->serverTargetPortLineEdit;

    refresh();
}

SAKWebSocketClientDeviceController::~SAKWebSocketClientDeviceController()
{
    delete ui;
}

QString SAKWebSocketClientDeviceController::localHost()
{
    uiMutex.lock();
    QString ret = localhostComboBox->currentText();
    uiMutex.unlock();
    return ret;
}

quint16 SAKWebSocketClientDeviceController::localPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(localPortlineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

QString SAKWebSocketClientDeviceController::serverHost()
{
    uiMutex.lock();
    QString ret = serverHostLineEdit->text();
    uiMutex.unlock();
    return ret;
}

quint16 SAKWebSocketClientDeviceController::serverPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(serverPortLineEdit->text().toInt());
    uiMutex.unlock();

    return ret;
}

bool SAKWebSocketClientDeviceController::enableCustomLocalSetting()
{
    uiMutex.lock();
    bool ret = enableLocalSettingCheckBox->isChecked();
    uiMutex.unlock();
    return ret;
}

void SAKWebSocketClientDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(localhostComboBox);
}

void SAKWebSocketClientDeviceController::setUiEnable(bool enable)
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    serverHostLineEdit->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}
