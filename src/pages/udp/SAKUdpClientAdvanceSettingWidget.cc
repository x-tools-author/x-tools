/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>

#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientAdvanceSettingWidget.hh"
#include "SAKUdpClientMulticastEditingDialog.hh"

#include "ui_SAKUdpClientAdvanceSettingWidget.h"

SAKUdpClientAdvanceSettingWidget::SAKUdpClientAdvanceSettingWidget(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpClientAdvanceSettingWidget)
    ,udpDevice(Q_NULLPTR)
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

SAKUdpClientAdvanceSettingWidget::~SAKUdpClientAdvanceSettingWidget()
{
    delete ui;
}

void SAKUdpClientAdvanceSettingWidget::setUdpDevice(SAKUdpClientDevice *device)
{
    if (device){
        udpDevice = device;
        udpDevice->setUnicastEnable(unicastCheckBox->isChecked());
        udpDevice->setMulticastEnable(multicastCheckBox->isChecked());
        udpDevice->setBroadcastEnable(broadcastCheckBox->isChecked());
    }
}

void SAKUdpClientAdvanceSettingWidget::on_unicastCheckBox_clicked()
{
    if (!isInitUi){
        bool enable = unicastCheckBox->isChecked();
        udpDevice->setUnicastEnable(enable);
    }
}

void SAKUdpClientAdvanceSettingWidget::on_broadcastCheckBox_clicked()
{
    if (!isInitUi){
        bool enable = broadcastCheckBox->isChecked();
        broadcastPortLineEdit->setEnabled(!enable);
        udpDevice->setBroadcastEnable(enable);
        udpDevice->setBroadcastPort(quint16(broadcastPortLineEdit->text().toInt()));
    }
}

void SAKUdpClientAdvanceSettingWidget::on_multicastCheckBox_clicked()
{
    if (!isInitUi){
        bool enable = multicastCheckBox->isChecked();
        udpDevice->setBroadcastEnable(enable);
    }
}

void SAKUdpClientAdvanceSettingWidget::on_deletePushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (!item){
        QMessageBox::warning(this, tr("未选择组播条目"), tr("请选择一条组播后重试"));
        return;
    }

    QString text = item->text();
    QString address = text.split(':').first();
    quint16 port = text.split(':').last().toUShort();
    udpDevice->removeMulticastInfo(address, port);
    delete item;
}

void SAKUdpClientAdvanceSettingWidget::on_addPushButton_clicked()
{
    SAKUdpClientMulticastEditingDialog dialog;
    dialog.show();
    int ret = dialog.exec();
    if (ret == QDialog::Accepted){
        QString address = dialog.address();
        quint16 port = dialog.port();
        udpDevice->addMulticastInfo(address, port);

        listWidget->addItem(QString("%1:%2").arg(address).arg(port));
    }
}
