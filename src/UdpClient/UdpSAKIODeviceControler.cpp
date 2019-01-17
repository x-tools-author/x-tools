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
#include <QSettings>

UdpSAKIODeviceControler::UdpSAKIODeviceControler(QWidget *parent)
    :SAKIODeviceControler(parent)
    ,ui(new Ui::UdpSAKIODeviceControler)
{
    ui->setupUi(this);

    localHostAddress = ui->comboBoxHostAddress;
    localPort        = ui->lineEditPort;
    peerPort         = ui->lineEditPeerPort;
    peerAddress      = ui->lineEditPeerHostAddress;

    setObjectName(QString("UdpClientControler"));

    readPeerPort();
    readLocalePort();
    readPeerAddress();

    connect(localPort, SIGNAL(textChanged(QString)), this, SLOT(setLocalePort(QString)));
    connect(peerPort, SIGNAL(textChanged(QString)), this, SLOT(setPeerPort(QString)));
    connect(peerAddress, SIGNAL(textChanged(QString)), this, SLOT(setPeerAddress(QString)));

    initUi();
}

UdpSAKIODeviceControler::~UdpSAKIODeviceControler()
{

}

void UdpSAKIODeviceControler::afterDeviceOpen()
{
    localHostAddress->setEnabled(false);
    peerAddress->setEnabled(false);
    localPort->setEnabled(false);
    peerPort->setEnabled(false);
}

void UdpSAKIODeviceControler::afterDeviceClose()
{
    localHostAddress->setEnabled(true);
    peerAddress->setEnabled(true);
    localPort->setEnabled(true);
    peerPort->setEnabled(true);
}

void UdpSAKIODeviceControler::open()
{
    QString _localHostAddress = localHostAddress->currentText();
    QString _localPort = localPort->text();
    QString _peerHostAddress = peerAddress->text();
    QString _peerPort = peerPort->text();

    emit need2open(_localHostAddress, _localPort, _peerHostAddress, _peerPort);
}

void UdpSAKIODeviceControler::refresh()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

    localHostAddress->clear();
    if (addressList.isEmpty()){
        localHostAddress->addItem(tr("无可用网络设备"));
    }else {
        QList<QHostAddress> ipv4AddressList;
        foreach (QHostAddress address, addressList) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol){
                ipv4AddressList.append(address);
            }
        }

        foreach (QHostAddress address, ipv4AddressList) {
            localHostAddress->addItem(address.toString());
        }
    }
}

void UdpSAKIODeviceControler::initUi()
{
    refresh();
    peerPort->setValidator(new QIntValidator(0, 65535, this));
    localPort->setValidator(new QIntValidator(0, 65535, this));
}

void UdpSAKIODeviceControler::setLocalePort(QString port)
{
    QSettings settings;
    QString key = objectName() + "/" + LOCAL_PORT;

    settings.setValue(key, port);
}

void UdpSAKIODeviceControler::readLocalePort()
{
    QSettings settings;
    QString key = objectName() + "/" + LOCAL_PORT;
    QString value = settings.value(key).toString();

    localPort->setText(value);
}

void UdpSAKIODeviceControler::setPeerPort(QString port)
{
    QSettings settings;
    QString key = objectName() + "/" + PEER_PORT;

    settings.setValue(key, port);
}

void UdpSAKIODeviceControler::readPeerPort()
{
    QSettings settings;
    QString key = objectName() + "/" + PEER_PORT;
    QString value = settings.value(key).toString();

    peerPort->setText(value);
}

void UdpSAKIODeviceControler::setPeerAddress(QString address)
{
    QSettings settings;
    QString key = objectName() + "/" + PEER_ADDRESS;

    settings.setValue(key, address);
}

void UdpSAKIODeviceControler::readPeerAddress()
{
    QSettings settings;
    QString key = objectName() + "/" + PEER_ADDRESS;
    QString value = settings.value(key).toString();

    peerAddress->setText(value);
}
