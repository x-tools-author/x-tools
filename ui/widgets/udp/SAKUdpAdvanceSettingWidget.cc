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

#include "SAKUdpDevice.hh"
#include "SAKUdpAdvanceSettingWidget.hh"

#include "ui_SAKUdpAdvanceSettingWidget.h"

SAKUdpAdvanceSettingWidget::SAKUdpAdvanceSettingWidget(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpAdvanceSettingsWidget)
    ,isInitUi (true)
{
    ui->setupUi(this);

    unicastCheckBox = ui->unicastCheckBox;
    broadcastCheckBox = ui->broadcastCheckBox;
    broadcastPortLineEdit = ui->broadcastPortLineEdit;
    multicastCheckBox = ui->multicastCheckBox;
    listWidget = ui->listWidget;
    deletePushButton = ui->deletePushButton;
    addPushButton = ui->addPushButton;

    isInitUi = false;
}

SAKUdpAdvanceSettingWidget::~SAKUdpAdvanceSettingWidget()
{
    delete ui;
}

void SAKUdpAdvanceSettingWidget::setUdpDevice(SAKUdpDevice *device)
{
    Q_CHECK_PTR(device);
    udpDevice = device;
}

void SAKUdpAdvanceSettingWidget::on_unicastCheckBox_clicked()
{
    if (!isInitUi){
        bool enable = unicastCheckBox->isChecked();
        udpDevice->setUnicastEnable(enable);
    }
}

void SAKUdpAdvanceSettingWidget::on_broadcastCheckBox_clicked()
{
    if (!isInitUi){
        bool enable = broadcastCheckBox->isChecked();
        broadcastPortLineEdit->setEnabled(!enable);
        udpDevice->setBroadcastEnable(enable);
        udpDevice->setBroadcastPort(quint16(broadcastPortLineEdit->text().toInt()));
    }
}

void SAKUdpAdvanceSettingWidget::on_multicastCheckBox_clicked()
{

}

void SAKUdpAdvanceSettingWidget::on_deletePushButton_clicked()
{

}

void SAKUdpAdvanceSettingWidget::on_addPushButton_clicked()
{

}
