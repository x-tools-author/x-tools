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
    uiMutex.lock();
    QString ret = localhostComboBox->currentText();
    uiMutex.unlock();
    return ret;
}

quint16 SAKUdpDeviceController::localPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(localPortlineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

QString SAKUdpDeviceController::targetHost()
{
    uiMutex.lock();
    QString ret = targetHostLineEdit->text();
    uiMutex.unlock();
    return ret;
}

quint16 SAKUdpDeviceController::targetPort()
{
    uiMutex.lock();
    quint16 ret = static_cast<quint16>(targetPortLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

bool SAKUdpDeviceController::enableCustomLocalSetting()
{
    uiMutex.lock();
    bool ret = enableLocalSettingCheckBox->isChecked();
    uiMutex.unlock();
    return ret;
}

void SAKUdpDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(localhostComboBox, true);
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
