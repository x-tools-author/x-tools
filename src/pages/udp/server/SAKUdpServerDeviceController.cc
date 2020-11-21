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

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKUdpServerDeviceController.hh"
#include "ui_SAKUdpServerDeviceController.h"

SAKUdpServerDeviceController::SAKUdpServerDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKUdpServerDeviceController)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;

    on_clientHostComboBox_currentTextChanged(mClientHostComboBox->currentText());
    on_serverhostComboBox_currentTextChanged(mServerHostComboBox->currentText());
    on_serverPortLineEdit_textChanged(mServerPortLineEdit->text());
    qRegisterMetaType<SAKUdpServerDeviceController::UdpServerParameters>("SAKUdpServerDeviceController::UdpServerParameters");
    refreshDevice();
}

SAKUdpServerDeviceController::~SAKUdpServerDeviceController()
{
    delete mUi;
}

QVariant SAKUdpServerDeviceController::parameters()
{
    UdpServerParameters parameters;
    mParametersMutex.lock();
    parameters.serverHost = mParameters.serverHost;
    parameters.serverPort = mParameters.serverPort;
    parameters.currentClientHost = mParameters.currentClientHost;
    parameters.currentClientPort = mParameters.currentClientPort;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKUdpServerDeviceController::setUiEnable(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKUdpServerDeviceController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mServerHostComboBox);
}

void SAKUdpServerDeviceController::addClient(QString host, quint16 port)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    bool isItemExisted = false;
    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemText(i).compare(item) == 0){
            isItemExisted = true;
            break;
        }
    }

    if (!isItemExisted){
        mClientHostComboBox->addItem(item);
        mParametersMutex.lock();
        mParameters.clients.append(item);
        mParametersMutex.unlock();
    }

    mParametersMutex.lock();
    auto parameters = mParameters;
    mParametersMutex.unlock();
    if (parameters.currentClientHost.isEmpty()){
        on_clientHostComboBox_currentTextChanged(mClientHostComboBox->currentText());
    }
}

void SAKUdpServerDeviceController::on_clientHostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    QStringList infoList = arg1.trimmed().split(':');
    mParameters.currentClientHost = infoList.first();
    mParameters.currentClientPort = infoList.last().toInt();
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_clearPushButton_clicked()
{
    mClientHostComboBox->clear();
    mParametersMutex.lock();
    mParameters.currentClientHost.clear();
    mParameters.currentClientPort = 0;
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_serverhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = arg1.toUInt();
    mParametersMutex.unlock();
}
