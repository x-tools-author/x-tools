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
#include "SAKTcpClientDeviceController.hh"
#include "ui_SAKTcpClientDeviceController.h"
SAKTcpClientDeviceController::SAKTcpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKTcpClientDeviceController)
{
    mUi->setupUi(this);

    mLocalhostComboBox = mUi->localhostComboBox;
    mLocalPortlineEdit = mUi->localPortlineEdit;
    mSpecifyClientAddressAndPort = mUi->specifyClientAddressAndPort;
    mServerHostLineEdit = mUi->serverHostLineEdit;
    mServerPortLineEdit = mUi->serverPortLineEdit;

    qRegisterMetaType<SAKTcpClientDeviceController::TcpClientParameters>("SAKTcpClientDeviceController::TcpClientParameters");
    mParameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
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
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKTcpClientDeviceController::setUiEnable(bool enable)
{
    mLocalhostComboBox->setEnabled(enable);
    mLocalPortlineEdit->setEnabled(enable);
    mSpecifyClientAddressAndPort->setEnabled(enable);
    mServerHostLineEdit->setEnabled(enable);
    mServerPortLineEdit->setEnabled(enable);
}

void SAKTcpClientDeviceController::refreshDevice()
{
    SAKGlobal::initIpComboBox(mLocalhostComboBox);
}

QString SAKTcpClientDeviceController::localHost()
{
    mParametersMutex.lock();
    QString ret = mLocalhostComboBox->currentText();
    mParametersMutex.unlock();
    return ret;
}

quint16 SAKTcpClientDeviceController::localPort()
{
    mParametersMutex.lock();
    quint16 ret = static_cast<quint16>(mLocalPortlineEdit->text().toInt());
    mParametersMutex.unlock();
    return ret;
}

QString SAKTcpClientDeviceController::serverHost()
{
    mParametersMutex.lock();
    QString ret = mServerHostLineEdit->text();
    mParametersMutex.unlock();
    return ret;
}

quint16 SAKTcpClientDeviceController::serverPort()
{
    mParametersMutex.lock();
    quint16 ret = static_cast<quint16>(mServerPortLineEdit->text().toInt());
    mParametersMutex.unlock();

    return ret;
}

bool SAKTcpClientDeviceController::enableCustomLocalSetting()
{
    mParametersMutex.lock();
    bool ret = mSpecifyClientAddressAndPort->isChecked();
    mParametersMutex.unlock();
    return ret;
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
    mParameters.serverHost = static_cast<quint16>(arg1.toInt());
    mParametersMutex.unlock();
}

void SAKTcpClientDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = static_cast<quint16>(arg1.toInt());
    mParametersMutex.unlock();
}
