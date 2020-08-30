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
#include "SAKSslSocketServerDeviceController.hh"
#include "SAKDebugPageCommonSslConfigurationWidget.hh"

#include "ui_SAKSslSocketServerDeviceController.h"

SAKSslSocketServerDeviceController::SAKSslSocketServerDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKSslSocketServerDeviceController)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
    mSslConfigurationPushButton = mUi->sslConfigurationPushButton;

    on_serverhostComboBox_currentTextChanged(mServerHostComboBox->currentText());
    on_serverPortLineEdit_textChanged(mServerPortLineEdit->text());
    qRegisterMetaType<SAKSslSocketServerDeviceController::SslSocketServerParameters>("SAKSslSocketServerDeviceController::SslSocketServerParameters");
    refreshDevice();

    mClientHostComboBox->setModel(&mClientStandardItemModel);
    mSslConfiguration = new SAKDebugPageCommonSslConfigurationWidget(debugPage->settings());
}

SAKSslSocketServerDeviceController::~SAKSslSocketServerDeviceController()
{
    delete mUi;
    mSslConfiguration->deleteLater();
}

QVariant SAKSslSocketServerDeviceController::parameters()
{
    SslSocketServerParameters parameters;
    mParametersMutex.lock();
    parameters.serverHost = mParameters.serverHost;
    parameters.serverPort = mParameters.serverPort;
    parameters.currentClientHost = mParameters.currentClientHost;
    parameters.currentClientPort = mParameters.currentClientPort;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKSslSocketServerDeviceController::setUiEnable(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
    mSslConfigurationPushButton->setEnabled(!opened);
}

void SAKSslSocketServerDeviceController::refreshDevice()
{
    SAKGlobal::initIpComboBox(mServerHostComboBox);
}

void SAKSslSocketServerDeviceController::addClient(QString host, quint16 port, QTcpSocket *socket)
{
    QString itemString = host.append(":");
    itemString.append(QString::number(port));

    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemText(i).compare(itemString) == 0){
            mClientHostComboBox->removeItem(i);
            break;
        }
    }

    mClientHostComboBox->addItem(itemString, QVariant::fromValue(socket));
}

void SAKSslSocketServerDeviceController::removeClient(QTcpSocket *socket)
{
    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemData(i).value<QTcpSocket*>() == socket){
            mClientHostComboBox->removeItem(i);
            break;
        }
    }
}

void SAKSslSocketServerDeviceController::on_serverhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKSslSocketServerDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = arg1.toInt();
    mParametersMutex.unlock();
}

void SAKSslSocketServerDeviceController::on_clientHostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    QStringList info = arg1.split(":");
    mParameters.currentClientHost = info.first();
    mParameters.currentClientPort = info.last().toInt();
    mParametersMutex.unlock();
}

void SAKSslSocketServerDeviceController::on_sslConfigurationPushButton_clicked()
{
    if (mSslConfiguration->isHidden()){
        mSslConfiguration->show();
    }else{
        mSslConfiguration->activateWindow();
    }
}
