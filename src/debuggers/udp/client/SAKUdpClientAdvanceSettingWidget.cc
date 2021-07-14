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
    :QWidget(parent)
    ,mUdpDevice(Q_NULLPTR)
    ,mIsInitUi(true)
    ,mUi(new Ui::SAKUdpClientAdvanceSettingWidget)
{
    mUi->setupUi(this);
    mUnicastCheckBox = mUi->unicastCheckBox;
    mBroadcastCheckBox = mUi->broadcastCheckBox;
    mBroadcastPortLineEdit = mUi->broadcastPortLineEdit;
    multicastCheckBox = mUi->multicastCheckBox;
    mListWidget = mUi->listWidget;
    mDeletePushButton = mUi->deletePushButton;
    mAddPushButton = mUi->addPushButton;

    mIsInitUi = false;
}

SAKUdpClientAdvanceSettingWidget::~SAKUdpClientAdvanceSettingWidget()
{
    delete mUi;
}

void SAKUdpClientAdvanceSettingWidget::setUdpDevice(SAKUdpClientDevice *device)
{
    mUdpDevice = device;
    if (device){
        mUdpDevice->setUnicastEnable(mUnicastCheckBox->isChecked());
        mUdpDevice->setMulticastEnable(multicastCheckBox->isChecked());
        mUdpDevice->setBroadcastEnable(mBroadcastCheckBox->isChecked());
        mUdpDevice->setBroadcastPort(mBroadcastPortLineEdit->text().toInt());
    }
}

void SAKUdpClientAdvanceSettingWidget::on_unicastCheckBox_clicked()
{
    if (!mIsInitUi){
        bool enable = mUnicastCheckBox->isChecked();
        mUdpDevice->setUnicastEnable(enable);
    }
}

void SAKUdpClientAdvanceSettingWidget::on_broadcastCheckBox_clicked()
{
    if (!mIsInitUi){
        bool enable = mBroadcastCheckBox->isChecked();
        mBroadcastPortLineEdit->setEnabled(!enable);
        mUdpDevice->setBroadcastEnable(enable);
        mUdpDevice->setBroadcastPort(quint16(mBroadcastPortLineEdit->text().toInt()));
    }
}

void SAKUdpClientAdvanceSettingWidget::on_multicastCheckBox_clicked()
{
    if (!mIsInitUi){
        bool enable = multicastCheckBox->isChecked();
        mUdpDevice->setBroadcastEnable(enable);
    }
}

void SAKUdpClientAdvanceSettingWidget::on_deletePushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item){
        QMessageBox::warning(this, tr("No selected item"), tr("No selected item, please select an item and then try again."));
        return;
    }

    QString text = item->text();
    QString address = text.split(':').first();
    quint16 port = text.split(':').last().toUShort();
    mUdpDevice->removeMulticastInfo(address, port);
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

        QString errorString;
        bool success = mUdpDevice->joinMulticastGroup(address, port, errorString);
        if (success){
            mListWidget->addItem(QString("%1:%2").arg(address).arg(port));
        }else{
            QMessageBox::warning(this,
                                 tr("Join multicast group failed").arg(errorString),
                                 tr("Join multicast group failed:%1").arg(errorString));
        }
    }
}
