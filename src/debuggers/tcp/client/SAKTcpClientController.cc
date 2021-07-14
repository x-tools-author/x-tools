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
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKTcpClientController.hh"
#include "ui_SAKTcpClientController.h"

SAKTcpClientController::SAKTcpClientController(SAKDebugger *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKTcpClientController)
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

    qRegisterMetaType<SAKTcpClientController::TcpClientParameters>("SAKTcpClientController::TcpClientParameters");
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParameters.localPort = mLocalPortlineEdit->text().toInt();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParameters.serverHost = mServerHostLineEdit->text();
    mParameters.serverPort = mServerPortLineEdit->text().toInt();
    refreshDevice();
}

SAKTcpClientController::~SAKTcpClientController()
{
    delete mUi;
}

QVariant SAKTcpClientController::parameters()
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

void SAKTcpClientController::setUiEnable(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mServerHostLineEdit->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKTcpClientController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mLocalhostComboBox);
}

void SAKTcpClientController::setClientInfo(QString info)
{
    mClientInfoLineEdit->setText(info);
}

void SAKTcpClientController::on_localhostComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.localHost = mLocalhostComboBox->currentText();
    mParametersMutex.unlock();
}

void SAKTcpClientController::on_localPortlineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mParametersMutex.lock();
    mParameters.localPort = static_cast<quint16>(mLocalPortlineEdit->text().toInt());
    mParametersMutex.unlock();
}

void SAKTcpClientController::on_specifyClientAddressAndPort_clicked()
{
    mParametersMutex.lock();
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    mParametersMutex.unlock();
}

void SAKTcpClientController::on_serverHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKTcpClientController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = static_cast<quint16>(arg1.toInt());
    mParametersMutex.unlock();
}

void SAKTcpClientController::on_automaticConnectionCheckBox_clicked()
{
    mParametersMutex.lock();
    mParameters.allowAutomaticConnection = mUi->automaticConnectionCheckBox->isChecked();
    mParametersMutex.unlock();
}
