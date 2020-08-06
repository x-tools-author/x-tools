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
#include "SAKUdpServerDeviceController.hh"
#include "ui_SAKUdpServerDeviceController.h"

SAKUdpServerDeviceController::SAKUdpServerDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpServerDeviceController)
{
    ui->setupUi(this);

    serverHostComboBox = ui->serverhostComboBox;
    serverPortLineEdit = ui->serverPortLineEdit;
    clientHostComboBox = ui->clientHostComboBox;

    refresh();

    connect(this, &SAKUdpServerDeviceController::requestAddClient, this, &SAKUdpServerDeviceController::addClientSafelyActually);
}

SAKUdpServerDeviceController::~SAKUdpServerDeviceController()
{
    delete ui;
}

QString SAKUdpServerDeviceController::serverHost()
{
    mParametersMutex.lock();
    QString host = mServerHost;
    mParametersMutex.unlock();
    return host;
}

quint16 SAKUdpServerDeviceController::serverPort()
{
    mParametersMutex.lock();
    quint16 port = mServerPort;
    mParametersMutex.unlock();
    return port;
}

QString SAKUdpServerDeviceController::currentClientHost()
{
    mParametersMutex.lock();
    QString host = mCurrentHost;
    mParametersMutex.unlock();
    return host;
}

quint16 SAKUdpServerDeviceController::currentClientPort()
{
    mParametersMutex.lock();
    quint16 port = mCurrentPort;
    mParametersMutex.unlock();
    return port;
}

void SAKUdpServerDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(serverHostComboBox);
}

void SAKUdpServerDeviceController::setUiEnable(bool enable)
{
    serverHostComboBox->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}

void SAKUdpServerDeviceController::addClientSafely(QString host, quint16 port)
{
    emit requestAddClient(host, port);
}

void SAKUdpServerDeviceController::addClientSafelyActually(QString host, quint16 port)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    bool isItemExisted = false;
    for(int i = 0; i < clientHostComboBox->count(); i++){
        if (clientHostComboBox->itemText(i).compare(item) == 0){
            isItemExisted = true;
            break;
        }
    }

    if (!isItemExisted){
        clientHostComboBox->addItem(item);
    }
}

void SAKUdpServerDeviceController::on_clientHostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mCurrentHost = arg1.split(':').first();
    mCurrentPort = arg1.split(':').last().toInt();
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_clearPushButton_clicked()
{
    clientHostComboBox->clear();
    mParametersMutex.lock();
    mCurrentHost.clear();
    mCurrentPort = 0;
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_serverhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mServerHost = arg1;
    mParametersMutex.unlock();
}

void SAKUdpServerDeviceController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mServerPort = arg1.toUInt();
    mParametersMutex.unlock();
}
