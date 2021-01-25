/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKBluetoothServerDeviceController.hh"
#include "ui_SAKBluetoothServerDeviceController.h"

SAKBluetoothServerDeviceController::SAKBluetoothServerDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKBluetoothServerDeviceController)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;

    on_serverhostComboBox_currentTextChanged(mServerHostComboBox->currentText());
    on_serverPortLineEdit_textChanged(mServerPortLineEdit->text());
    qRegisterMetaType<SAKBluetoothServerDeviceController::BluetoothServerParameters>("SAKBluetoothServerDeviceController::BluetoothServerParameters");
    refreshDevice();

    mClientHostComboBox->setModel(&mClientStandardItemModel);
}

SAKBluetoothServerDeviceController::~SAKBluetoothServerDeviceController()
{
    delete mUi;
}

QVariant SAKBluetoothServerDeviceController::parameters()
{
    BluetoothServerParameters parameters;
    mParametersMutex.lock();
    parameters.serverHost = mParameters.serverHost;
    parameters.serverPort = mParameters.serverPort;
    parameters.currentClientHost = mParameters.currentClientHost;
    parameters.currentClientPort = mParameters.currentClientPort;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKBluetoothServerDeviceController::setUiEnable(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKBluetoothServerDeviceController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mServerHostComboBox);
}

void SAKBluetoothServerDeviceController::addClient(QString host, quint16 port, QTcpSocket *socket)
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

void SAKBluetoothServerDeviceController::removeClient(QTcpSocket *socket)
{
    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemData(i).value<QTcpSocket*>() == socket){
            mClientHostComboBox->removeItem(i);
            break;
        }
    }
}

void SAKBluetoothServerDeviceController::on_serverhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKBluetoothServerDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = arg1.toInt();
    mParametersMutex.unlock();
}

void SAKBluetoothServerDeviceController::on_clientHostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    QStringList info = arg1.split(":");
    mParameters.currentClientHost = info.first();
    mParameters.currentClientPort = info.last().toInt();
    mParametersMutex.unlock();
}
