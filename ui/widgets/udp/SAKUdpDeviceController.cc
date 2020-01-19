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
#include "SAKUdpDevice.hh"
#include "SAKUdpDeviceController.hh"
#include "SAKUdpAdvanceSettingWidget.hh"

#include "ui_SAKUdpDeviceController.h"
SAKUdpDeviceController::SAKUdpDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpDeviceController)
    ,udpAdvanceSettingWidget (new SAKUdpAdvanceSettingWidget)
{
    ui->setupUi(this);

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    targetHostLineEdit = ui->targetHostLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;
    advanceUdpPushButton = ui->advanceUdpPushButton;
    advanceUdpPushButton->setEnabled(false);

    refresh();
}

SAKUdpDeviceController::~SAKUdpDeviceController()
{
    delete udpAdvanceSettingWidget;
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
    advanceUdpPushButton->setEnabled(!enable);
}

void SAKUdpDeviceController::setUdpDevice(SAKUdpDevice *device)
{
    udpAdvanceSettingWidget->setUdpDevice(device);
}

void SAKUdpDeviceController::on_advanceUdpPushButton_clicked()
{
    if (udpAdvanceSettingWidget){
        udpAdvanceSettingWidget->isHidden() ? udpAdvanceSettingWidget->show() : udpAdvanceSettingWidget->activateWindow();
    }
}
