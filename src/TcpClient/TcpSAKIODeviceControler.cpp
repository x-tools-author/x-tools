/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include <QHostAddress>
#include <QNetworkInterface>

#include "TcpSAKIODeviceControler.h"

#include "SAKSettings.h"
#include "ui_TcpSAKIODeviceControler.h"

TcpSAKIODeviceControler::TcpSAKIODeviceControler(QWidget *parent)
    :SAKIODeviceControler(parent)
    ,ui(new Ui::TcpSAKIODeviceControler)
{
    ui->setupUi(this);
    initUi();

    connect(ui->lineEditLocalHostPort, SIGNAL(textChanged(QString)), this, SLOT(setLocalePort(QString)));
    connect(ui->lineEditServerPort, SIGNAL(textChanged(QString)), this, SLOT(setPeerPort(QString)));
    connect(ui->lineEditServerAddress, SIGNAL(textChanged(QString)), this, SLOT(setPeerAddress(QString)));
}

TcpSAKIODeviceControler::~TcpSAKIODeviceControler()
{

}

void TcpSAKIODeviceControler::afterDeviceOpen()
{
    ui->checkBoxAppointLocalHostAddress->setEnabled(false);
    ui->comboBoxLocalHostAddress->setEnabled(false);
    ui->lineEditLocalHostPort->setEnabled(false);
    ui->lineEditServerAddress->setEnabled(false);
    ui->lineEditServerPort->setEnabled(false);
}

void TcpSAKIODeviceControler::afterDeviceClose()
{
    ui->checkBoxAppointLocalHostAddress->setEnabled(true);
    ui->comboBoxLocalHostAddress->setEnabled(true);
    ui->lineEditLocalHostPort->setEnabled(true);
    ui->lineEditServerAddress->setEnabled(true);
    ui->lineEditServerPort->setEnabled(true);
}

void TcpSAKIODeviceControler::open()
{
    QString hostAddress = ui->comboBoxLocalHostAddress->currentText();
    QString hostPort = ui->lineEditLocalHostPort->text();
    QString serverPort = ui->lineEditServerPort->text();
    QString serverAddress = ui->lineEditServerAddress->text();

    if (ui->checkBoxAppointLocalHostAddress->isChecked()){
        emit need2open(hostAddress, hostPort, serverAddress, serverPort);
    }else {
        emit need2open(QString(""), hostPort, serverAddress, serverPort);
    }
}

void TcpSAKIODeviceControler::refresh()
{
    emit need2refresh();
}

void TcpSAKIODeviceControler::initUi()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

    ui->comboBoxLocalHostAddress->clear();
    if (addressList.isEmpty()){
        ui->comboBoxLocalHostAddress->addItem(tr("无可用网络设备"));
    }else {
        QList<QHostAddress> ipv4AddressList;
        foreach (QHostAddress address, addressList) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol){
                ipv4AddressList.append(address);
            }
        }

        foreach (QHostAddress address, ipv4AddressList) {
            ui->comboBoxLocalHostAddress->addItem(address.toString());
        }
    }

    ui->lineEditLocalHostPort->setText(sakSettings()->valueTcpClientLocalPort());
    ui->lineEditServerPort->setText(sakSettings()->valueTcpClientPeerPort());
    ui->lineEditServerAddress->setText(sakSettings()->valueTcpClientPeerAddress());
}

void TcpSAKIODeviceControler::setLocalePort(QString port)
{
    if (sakSettings() == NULL){
        qWarning("Setting function is not initialized!");
    }else{
        sakSettings()->setValueTcpClientLocalPort(port);
    }
}

void TcpSAKIODeviceControler::setPeerPort(QString port)
{
    if (sakSettings() == NULL){
        qWarning("Setting function is not initialized!");
    }else{
        sakSettings()->setValueTcpClientPeerPort(port);
    }
}

void TcpSAKIODeviceControler::setPeerAddress(QString address)
{
    if (sakSettings() == NULL){
        qWarning("Setting function is not initialized!");
    }else{
        sakSettings()->setValueTcpClientPeerAddress(address);
    }
}
