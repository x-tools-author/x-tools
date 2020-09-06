/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKSslSocketClientDeviceController.hh"
#include "SAKDebugPageCommonSslConfigurationWidget.hh"

#include "ui_SAKSslSocketClientDeviceController.h"

SAKSslSocketClientDeviceController::SAKSslSocketClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mSslConfigurationWidget(Q_NULLPTR)
    ,mUi(new Ui::SAKSslSocketClientDeviceController)
{
    mUi->setupUi(this);

    mLocalhostComboBox = mUi->localhostComboBox;
    mLocalPortlineEdit = mUi->localPortlineEdit;
    mSpecifyClientAddressAndPort = mUi->specifyClientAddressAndPort;
    mClientInfoLineEdit = mUi->clientInfoLineEdit;
    mServerHostLineEdit = mUi->serverHostLineEdit;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mSslConfigurationPushButton = mUi->sslConfigurationPushButton;

    qRegisterMetaType<SAKSslSocketClientDeviceController::SslSocketClientParameters>("SAKSslSocketClientDeviceController::SslSocketClientParameters");
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParameters.localPort = mLocalPortlineEdit->text().toInt();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParameters.serverHost = mServerHostLineEdit->text();
    mParameters.serverPort = mServerPortLineEdit->text().toInt();
    refreshDevice();

    mSslConfigurationWidget = new SAKDebugPageCommonSslConfigurationWidget(debugPage->settings());
}

SAKSslSocketClientDeviceController::~SAKSslSocketClientDeviceController()
{
    delete mUi;
    mSslConfigurationWidget->deleteLater();
}

QVariant SAKSslSocketClientDeviceController::parameters()
{
    SslSocketClientParameters parameters;
    mParametersMutex.lock();
    parameters.localHost = mParameters.localHost;
    parameters.localPort = mParameters.localPort;
    parameters.serverHost = mParameters.serverHost;
    parameters.serverPort = mParameters.serverPort;
    parameters.specifyClientAddressAndPort = mParameters.specifyClientAddressAndPort;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKSslSocketClientDeviceController::setUiEnable(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mServerHostLineEdit->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKSslSocketClientDeviceController::refreshDevice()
{
    SAKGlobal::initIpComboBox(mLocalhostComboBox);
}

void SAKSslSocketClientDeviceController::setClientInfo(QString info)
{
    mClientInfoLineEdit->setText(info);
}

SAKDebugPageCommonSslConfigurationWidget *SAKSslSocketClientDeviceController::sslConfigurationWidget()
{
    return mSslConfigurationWidget;
}

void SAKSslSocketClientDeviceController::on_localhostComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParametersMutex.unlock();
}

void SAKSslSocketClientDeviceController::on_localPortlineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mParametersMutex.lock();
    mParameters.localPort = static_cast<quint16>(mLocalPortlineEdit->text().toInt());
    mParametersMutex.unlock();
}

void SAKSslSocketClientDeviceController::on_specifyClientAddressAndPort_clicked()
{
    mParametersMutex.lock();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParametersMutex.unlock();
}

void SAKSslSocketClientDeviceController::on_serverHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKSslSocketClientDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = static_cast<quint16>(arg1.toInt());
    mParametersMutex.unlock();
}

void SAKSslSocketClientDeviceController::on_sslConfigurationPushButton_clicked()
{
    if(mSslConfigurationWidget->isHidden()){
        mSslConfigurationWidget->show();
    }else{
        mSslConfigurationWidget->activateWindow();
    }
}
