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
#include <QSerialPortInfo>

#include "SAKBase.hh"
#include "SAKUsbDeviceController.hh"
#include "ui_SAKUsbDeviceController.h"
SAKUsbDeviceController::SAKUsbDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUsbDeviceController)
{
    ui->setupUi(this);

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    targetHostLineEdit = ui->targetHostLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;

    refresh();
}

SAKUsbDeviceController::~SAKUsbDeviceController()
{
    delete ui;
}

QString SAKUsbDeviceController::localHost()
{
    return localhostComboBox->currentText();
}

quint16 SAKUsbDeviceController::localPort()
{
    return static_cast<quint16>(localPortlineEdit->text().toInt());
}

QString SAKUsbDeviceController::targetHost()
{
    return targetHostLineEdit->text();
}

quint16 SAKUsbDeviceController::targetPort()
{
    return static_cast<quint16>(targetPortLineEdit->text().toInt());
}

bool SAKUsbDeviceController::enableCustomLocalSetting()
{
    return enableLocalSettingCheckBox->isChecked();
}

void SAKUsbDeviceController::refresh()
{
    SAKBase::instance()->initIpComboBox(localhostComboBox);
}

void SAKUsbDeviceController::setUiEnable(bool enable)
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    targetHostLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
