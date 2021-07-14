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

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientDeviceController.hh"
#include "SAKUdpClientAdvanceSettingWidget.hh"

#include "ui_SAKUdpClientDeviceController.h"

SAKUdpClientDeviceController::SAKUdpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUdpAdvanceSettingWidget(new SAKUdpClientAdvanceSettingWidget)
    ,mUi(new Ui::SAKUdpClientDeviceController)
{
    mUi->setupUi(this);
    mLocalhostComboBox = mUi->localhostComboBox;
    mLocalPortlineEdit = mUi->localPortlineEdit;
    mSpecifyClientAddressAndPort = mUi->specifyClientAddressAndPort;
    mBoundInfoLineEdit = mUi->boundInfoLineEdit;
    mTargetHostLineEdit = mUi->targetHostLineEdit;
    mTargetPortLineEdit = mUi->targetPortLineEdit;
    mAdvanceUdpPushButton = mUi->advanceUdpPushButton;
    mAdvanceUdpPushButton->setEnabled(false);

    mParameters.localHost = mLocalhostComboBox->currentText();
    mParameters.localPort = mLocalPortlineEdit->text().toInt();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParameters.targetHost = mTargetHostLineEdit->text();
    mParameters.targetPort = mTargetPortLineEdit->text().toInt();
    qRegisterMetaType<SAKUdpClientDeviceController::UdpClientParameters>("SAKUdpClientDeviceController::UdpClientParameters");
    refreshDevice();
}

SAKUdpClientDeviceController::~SAKUdpClientDeviceController()
{
    delete mUdpAdvanceSettingWidget;
    delete mUi;
}

QVariant SAKUdpClientDeviceController::parameters()
{
    mParametersMutex.lock();
    auto parameter = mParameters;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameter);
}

void SAKUdpClientDeviceController::setUiEnable(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mTargetHostLineEdit->setEnabled(!opened);
    mTargetPortLineEdit->setEnabled(!opened);
    mAdvanceUdpPushButton->setEnabled(opened);
}

void SAKUdpClientDeviceController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mLocalhostComboBox, true);
}

void SAKUdpClientDeviceController::setUdpDevice(SAKUdpClientDevice *device)
{
    mUdpAdvanceSettingWidget->setUdpDevice(device);
}

void SAKUdpClientDeviceController::setClientInfo(QString info)
{
    mBoundInfoLineEdit->setText(info);
}

void SAKUdpClientDeviceController::on_advanceUdpPushButton_clicked()
{
    if (mUdpAdvanceSettingWidget){
        mUdpAdvanceSettingWidget->isHidden() ? mUdpAdvanceSettingWidget->show() : mUdpAdvanceSettingWidget->activateWindow();
    }
}

void SAKUdpClientDeviceController::on_localhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.localHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpClientDeviceController::on_localPortlineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.localPort = arg1.toInt();
    mParametersMutex.unlock();
}

void SAKUdpClientDeviceController::on_targetHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.targetHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpClientDeviceController::on_targetPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.targetPort = arg1.toInt();
    mParametersMutex.unlock();
}
