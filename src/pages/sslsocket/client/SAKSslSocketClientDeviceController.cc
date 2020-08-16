/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKTcpClientDeviceController.hh"
#include "ui_SAKTcpClientDeviceController.h"
SAKTcpClientDeviceController::SAKTcpClientDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKTcpClientDeviceController)
{
    ui->setupUi(this);

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    serverHostLineEdit = ui->serverHostLineEdit;
    serverPortLineEdit = ui->serverTargetPortLineEdit;

    refresh();
}

SAKTcpClientDeviceController::~SAKTcpClientDeviceController()
{
    delete ui;
}

QString SAKTcpClientDeviceController::localHost()
{
    uiMutex.lock();
    QString ret = localhostComboBox->currentText();
    uiMutex.unlock();
    return ret;
}

quint16 SAKTcpClientDeviceController::localPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(localPortlineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

QString SAKTcpClientDeviceController::serverHost()
{
    uiMutex.lock();
    QString ret = serverHostLineEdit->text();
    uiMutex.unlock();
    return ret;
}

quint16 SAKTcpClientDeviceController::serverPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(serverPortLineEdit->text().toInt());
    uiMutex.unlock();

    return ret;
}

bool SAKTcpClientDeviceController::enableCustomLocalSetting()
{
    uiMutex.lock();
    bool ret = enableLocalSettingCheckBox->isChecked();
    uiMutex.unlock();
    return ret;
}

void SAKTcpClientDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(localhostComboBox);
}

void SAKTcpClientDeviceController::setUiEnable(bool enable)
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    serverHostLineEdit->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}
