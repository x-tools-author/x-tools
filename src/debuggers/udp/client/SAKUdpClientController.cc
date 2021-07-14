/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientController.hh"
#include "SAKUdpClientAdvanceSettingWidget.hh"

#include "ui_SAKUdpClientController.h"

SAKUdpClientController::SAKUdpClientController(SAKDebugger *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUdpAdvanceSettingWidget(new SAKUdpClientAdvanceSettingWidget)
    ,mUi(new Ui::SAKUdpClientController)
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
    qRegisterMetaType<SAKUdpClientController::UdpClientParameters>("SAKUdpClientController::UdpClientParameters");
    refreshDevice();
}

SAKUdpClientController::~SAKUdpClientController()
{
    delete mUdpAdvanceSettingWidget;
    delete mUi;
}

QVariant SAKUdpClientController::parameters()
{
    mParametersMutex.lock();
    auto parameter = mParameters;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameter);
}

void SAKUdpClientController::setUiEnable(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mTargetHostLineEdit->setEnabled(!opened);
    mTargetPortLineEdit->setEnabled(!opened);
    mAdvanceUdpPushButton->setEnabled(opened);
}

void SAKUdpClientController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mLocalhostComboBox, true);
}

void SAKUdpClientController::setUdpDevice(SAKUdpClientDevice *device)
{
    mUdpAdvanceSettingWidget->setUdpDevice(device);
}

void SAKUdpClientController::setClientInfo(QString info)
{
    mBoundInfoLineEdit->setText(info);
}

void SAKUdpClientController::on_advanceUdpPushButton_clicked()
{
    if (mUdpAdvanceSettingWidget){
        mUdpAdvanceSettingWidget->isHidden() ? mUdpAdvanceSettingWidget->show() : mUdpAdvanceSettingWidget->activateWindow();
    }
}

void SAKUdpClientController::on_localhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.localHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpClientController::on_localPortlineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.localPort = arg1.toInt();
    mParametersMutex.unlock();
}

void SAKUdpClientController::on_targetHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.targetHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpClientController::on_targetPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.targetPort = arg1.toInt();
    mParametersMutex.unlock();
}
