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

#include "UdpSAKIODeviceControler.h"
#include "ui_UdpSAKIODeviceControler.h"

#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QIntValidator>

UdpSAKIODeviceControler::UdpSAKIODeviceControler(QWidget *parent)
    :SAKIODeviceControler(parent)
    ,ui(new Ui::UdpSAKIODeviceControler)
{
    ui->setupUi(this);
    initUi();
}

UdpSAKIODeviceControler::~UdpSAKIODeviceControler()
{

}

void UdpSAKIODeviceControler::afterDeviceOpen()
{
    ui->comboBoxHostAddress->setEnabled(false);
    ui->lineEditPeerHostAddress->setEnabled(false);
    ui->lineEditPeerPort->setEnabled(false);
    ui->lineEditPort->setEnabled(false);
}

void UdpSAKIODeviceControler::afterDeviceClose()
{
    ui->comboBoxHostAddress->setEnabled(true);
    ui->lineEditPeerHostAddress->setEnabled(true);
    ui->lineEditPeerPort->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
}

void UdpSAKIODeviceControler::open()
{
    QString localHostAddress = ui->comboBoxHostAddress->currentText();
    QString localPort = ui->lineEditPort->text();
    QString peerHostAddress = ui->lineEditPeerHostAddress->text();
    QString peerPort = ui->lineEditPeerPort->text();

    emit need2open(localHostAddress, localPort, peerHostAddress, peerPort);
}

void UdpSAKIODeviceControler::refresh()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

    ui->comboBoxHostAddress->clear();
    if (addressList.isEmpty()){
        ui->comboBoxHostAddress->addItem(tr("无可用网络设备"));
    }else {
        QList<QHostAddress> ipv4AddressList;
        foreach (QHostAddress address, addressList) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol){
                ipv4AddressList.append(address);
            }
        }

        foreach (QHostAddress address, ipv4AddressList) {
            ui->comboBoxHostAddress->addItem(address.toString());
        }
    }
}

void UdpSAKIODeviceControler::initUi()
{
    refresh();
    ui->lineEditPeerPort->setValidator(new QIntValidator(0, 65535, this));
    ui->lineEditPort->setValidator(new QIntValidator(0, 65535, this));
}
