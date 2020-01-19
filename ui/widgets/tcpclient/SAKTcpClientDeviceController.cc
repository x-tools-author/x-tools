/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
    return localhostComboBox->currentText();
}

quint16 SAKTcpClientDeviceController::localPort()
{
    return static_cast<quint16>(localPortlineEdit->text().toInt());
}

QString SAKTcpClientDeviceController::serverHost()
{
    return serverHostLineEdit->text();
}

quint16 SAKTcpClientDeviceController::serverPort()
{
    return static_cast<quint16>(serverPortLineEdit->text().toInt());
}

bool SAKTcpClientDeviceController::enableCustomLocalSetting()
{
    return enableLocalSettingCheckBox->isChecked();
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
