/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKUdpDeviceController.hh"
#include "ui_SAKUdpDeviceController.h"
SAKUdpDeviceController::SAKUdpDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpDeviceController)
{
    ui->setupUi(this);

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    targetHostLineEdit = ui->targetHostLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;

    refresh();
}

SAKUdpDeviceController::~SAKUdpDeviceController()
{
    delete ui;
}

QString SAKUdpDeviceController::localHost()
{
    return localhostComboBox->currentText();
}

quint16 SAKUdpDeviceController::localPort()
{
    return static_cast<quint16>(localPortlineEdit->text().toInt());
}

QString SAKUdpDeviceController::targetHost()
{
    return targetHostLineEdit->text();
}

quint16 SAKUdpDeviceController::targetPort()
{
    return static_cast<quint16>(targetPortLineEdit->text().toInt());
}

bool SAKUdpDeviceController::enableCustomLocalSetting()
{
    return enableLocalSettingCheckBox->isChecked();
}

void SAKUdpDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(localhostComboBox);
}

void SAKUdpDeviceController::setUiEnable(bool enable)
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    targetHostLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
