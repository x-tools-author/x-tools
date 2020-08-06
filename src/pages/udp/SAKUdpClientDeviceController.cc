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
#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientDeviceController.hh"
#include "SAKUdpClientAdvanceSettingWidget.hh"

#include "ui_SAKUdpClientDeviceController.h"

SAKUdpClientDeviceController::SAKUdpClientDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpClientDeviceController)
    ,udpAdvanceSettingWidget (new SAKUdpClientAdvanceSettingWidget)
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

SAKUdpClientDeviceController::~SAKUdpClientDeviceController()
{
    delete udpAdvanceSettingWidget;
    delete ui;
}

QString SAKUdpClientDeviceController::localHost()
{
    uiMutex.lock();
    QString ret = localhostComboBox->currentText();
    uiMutex.unlock();
    return ret;
}

quint16 SAKUdpClientDeviceController::localPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(localPortlineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

QString SAKUdpClientDeviceController::targetHost()
{
    uiMutex.lock();
    QString ret = targetHostLineEdit->text();
    uiMutex.unlock();
    return ret;
}

quint16 SAKUdpClientDeviceController::targetPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(targetPortLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

bool SAKUdpClientDeviceController::enableCustomLocalSetting()
{
    uiMutex.lock();
    bool ret = enableLocalSettingCheckBox->isChecked();
    uiMutex.unlock();
    return ret;
}

void SAKUdpClientDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(localhostComboBox, true);
}

void SAKUdpClientDeviceController::setUiEnable(bool enable)
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    targetHostLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
    advanceUdpPushButton->setEnabled(!enable);
}

void SAKUdpClientDeviceController::setUdpDevice(SAKUdpClientDevice *device)
{
    udpAdvanceSettingWidget->setUdpDevice(device);
}

void SAKUdpClientDeviceController::on_advanceUdpPushButton_clicked()
{
    if (udpAdvanceSettingWidget){
        udpAdvanceSettingWidget->isHidden() ? udpAdvanceSettingWidget->show() : udpAdvanceSettingWidget->activateWindow();
    }
}
