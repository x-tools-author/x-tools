/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "TcpServerSAKIODeviceControler.h"

#include "ui_TcpServerSAKIODeviceControler.h"

#include <QNetworkInterface>
#include <QSettings>

TcpServerSAKIODeviceControler::TcpServerSAKIODeviceControler(QWidget *parent)
    :SAKIODeviceControler(parent)
    ,ui(new Ui::TcpServerSAKIODeviceControler)
{
    setObjectName(QString("TCPServerController"));

    ui->setupUi(this);

    initUi();

    connect(ui->comboBoxClients, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentClient(QString)));
    connect(ui->lineEditServerPort, SIGNAL(textChanged(QString)), this, SLOT(setServerPort(QString)));

    readServerPort();
}

TcpServerSAKIODeviceControler::~TcpServerSAKIODeviceControler()
{

}

void TcpServerSAKIODeviceControler::afterDeviceOpen()
{
    ui->comboBoxServerHostAddress->setEnabled(false);
    ui->lineEditServerPort->setEnabled(false);
}

void TcpServerSAKIODeviceControler::afterDeviceClose()
{
    ui->comboBoxServerHostAddress->setEnabled(true);
    ui->lineEditServerPort->setEnabled(true);
}

void TcpServerSAKIODeviceControler::open()
{
    emit need2open(ui->comboBoxServerHostAddress->currentText(), ui->lineEditServerPort->text());
}


void TcpServerSAKIODeviceControler::refresh()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

    ui->comboBoxServerHostAddress->clear();
    if (addressList.isEmpty()){
        ui->comboBoxServerHostAddress->addItem(tr("无可用网络设备"));
    }else {
        QList<QHostAddress> ipv4AddressList;
        foreach (QHostAddress address, addressList) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol){
                ipv4AddressList.append(address);
            }
        }

        foreach (QHostAddress address, ipv4AddressList) {
            ui->comboBoxServerHostAddress->addItem(address.toString());
        }
    }
}

void TcpServerSAKIODeviceControler::initUi()
{
    refresh();
}

void TcpServerSAKIODeviceControler::changedClients(QList<QTcpSocket *> clients)
{
    disconnect(ui->comboBoxClients, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentClient(QString)));
    if (clients.isEmpty()){
        ui->comboBoxClients->clear();
        ui->comboBoxClients->addItem(tr("等待客户端连接"));
    }else {
        ui->comboBoxClients->clear();
        foreach(QTcpSocket *client, clients){
            ui->comboBoxClients->addItem(client->peerAddress().toString() + ":" + QString::number(client->peerPort()));
        }
    }
    connect(ui->comboBoxClients, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentClient(QString)));
}

void TcpServerSAKIODeviceControler::setCurrentClient(QString text)
{
    QString address, port;
    QStringList list = text.split(':');
    address = list.at(0);
    port = list.at(1);
    emit need2changeCurrentClients(address, port);
}

void TcpServerSAKIODeviceControler::setServerPort(QString port)
{
    QSettings settings;
    QString key = objectName() + "/" + QString("PeerPort");

    settings.setValue(key, port);
}

void TcpServerSAKIODeviceControler::readServerPort()
{
    QSettings settings;
    QString key = objectName() + "/" + QString("PeerPort");
    QString value = settings.value(key).toString();

    ui->lineEditServerPort->setText(value);
}
