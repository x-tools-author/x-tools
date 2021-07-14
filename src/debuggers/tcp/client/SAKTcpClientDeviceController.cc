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
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKTcpClientDeviceController.hh"
#include "ui_SAKTcpClientDeviceController.h"

SAKTcpClientDeviceController::SAKTcpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKTcpClientDeviceController)
{
    mUi->setupUi(this);

    mParameters.allowAutomaticConnection = false;
    mLocalhostComboBox = mUi->localhostComboBox;
    mLocalPortlineEdit = mUi->localPortlineEdit;
    mSpecifyClientAddressAndPort = mUi->specifyClientAddressAndPort;
    mAutomaticConnectionCheckBox = mUi->automaticConnectionCheckBox;
    mClientInfoLineEdit = mUi->clientInfoLineEdit;
    mServerHostLineEdit = mUi->serverHostLineEdit;
    mServerPortLineEdit = mUi->serverPortLineEdit;

    qRegisterMetaType<SAKTcpClientDeviceController::TcpClientParameters>("SAKTcpClientDeviceController::TcpClientParameters");
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParameters.localPort = mLocalPortlineEdit->text().toInt();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParameters.serverHost = mServerHostLineEdit->text();
    mParameters.serverPort = mServerPortLineEdit->text().toInt();
    refreshDevice();
}

SAKTcpClientDeviceController::~SAKTcpClientDeviceController()
{
    delete mUi;
}

QVariant SAKTcpClientDeviceController::parameters()
{
    TcpClientParameters parameters;
    mParametersMutex.lock();
    parameters.localHost = mParameters.localHost;
    parameters.localPort = mParameters.localPort;
    parameters.serverHost = mParameters.serverHost;
    parameters.serverPort = mParameters.serverPort;
    parameters.specifyClientAddressAndPort = mParameters.specifyClientAddressAndPort;
    parameters.allowAutomaticConnection = mParameters.allowAutomaticConnection;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKTcpClientDeviceController::setUiEnable(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mServerHostLineEdit->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKTcpClientDeviceController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mLocalhostComboBox);
}

void SAKTcpClientDeviceController::setClientInfo(QString info)
{
    mClientInfoLineEdit->setText(info);
}

void SAKTcpClientDeviceController::on_localhostComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_localPortlineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mParametersMutex.lock();
    mParameters.localPort = static_cast<quint16>(mLocalPortlineEdit->text().toInt());
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_specifyClientAddressAndPort_clicked()
{
    mParametersMutex.lock();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_serverHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = static_cast<quint16>(arg1.toInt());
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_automaticConnectionCheckBox_clicked()
{
    mParametersMutex.lock();
    mParameters.allowAutomaticConnection = mUi->automaticConnectionCheckBox->isChecked();
    mParametersMutex.unlock();
}
