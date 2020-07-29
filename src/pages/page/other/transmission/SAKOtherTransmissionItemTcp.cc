/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QHostAddress>

#include "SAKGlobal.hh"
#include "SAKOtherTransmissionItemTcp.hh"

#include "ui_SAKOtherTransmissionItemTcp.h"

SAKOtherTransmissionItemTcp::SAKOtherTransmissionItemTcp(SAKDebugPage *_debugPage, QWidget *parent)
    :SAKOtherTransmissionItem (_debugPage, parent)
    ,ui (new Ui::SAKOtherTransmissionItemTcp)
    ,tcpSocket (Q_NULLPTR)
{
    ui->setupUi(this);

    enableCheckBox = ui->enableCheckBox;
    customAddressCheckBox = ui->customAddressCheckBox;
    localAddressComboBox = ui->addressComboBox;
    localPortLineEdit = ui->portLineEdit;
    handleReceiveDataCheckBox = ui->handleReceiveDataCheckBox;
    serverAddressLineEdit = ui->targetAddressLineEdit;
    serverPortLineEdit = ui->targetPortLineEdit;

    SAKGlobal::initIpComboBox(localAddressComboBox);
}

SAKOtherTransmissionItemTcp::~SAKOtherTransmissionItemTcp()
{
    if (tcpSocket){
        delete tcpSocket;
    }
    delete ui;
}

void SAKOtherTransmissionItemTcp::write(QByteArray data)
{
    if (tcpSocket){
        if (!tcpSocket->write(data)){
#ifdef QT_DEBUG
        qDebug() << "发送取数据失败" << tcpSocket->errorString();
#endif
        }
    }
}

void SAKOtherTransmissionItemTcp::on_enableCheckBox_clicked()
{
    auto closeDev = [&](){
        if (tcpSocket){
            tcpSocket->disconnectFromHost();
            if (tcpSocket->state() == QTcpSocket::ConnectedState){
                tcpSocket->waitForDisconnected();
            }
            disconnect(tcpSocket, &QTcpSocket::readyRead, this, &SAKOtherTransmissionItemTcp::read);
            delete tcpSocket;
            tcpSocket = Q_NULLPTR;
            this->setUiEnable(true);
        }
    };

    if (enableCheckBox->isChecked()){
        tcpSocket = new QTcpSocket;
        if (customAddressCheckBox->isChecked()){
             if (!tcpSocket->bind(QHostAddress(localAddressComboBox->currentText()), static_cast<quint16>(localPortLineEdit->text().toInt()))){
                emit requestOutputMessage(tr("设备绑定失败：") + tcpSocket->errorString(), false);
                enableCheckBox->setChecked(false);
                closeDev();
                return;
             }
        }

        if (!tcpSocket->open(QTcpSocket::ReadWrite)){
            emit requestOutputMessage(tr("设备无法打开：") + tcpSocket->errorString(), false);
            enableCheckBox->setChecked(false);
            closeDev();
            return;
        }

        connect(tcpSocket, &QTcpSocket::readyRead, this, &SAKOtherTransmissionItemTcp::read);
        tcpSocket->connectToHost(serverAddressLineEdit->text(), static_cast<quint16>(serverPortLineEdit->text().toInt()));
        if (!tcpSocket->waitForConnected()){
            emit requestOutputMessage(tr("无法连接服务器：") + tcpSocket->errorString(), false);
            enableCheckBox->setChecked(false);            
            closeDev();
        }else{
            this->setUiEnable(false);
        }
    }else{
        closeDev();
    }
}

void SAKOtherTransmissionItemTcp::read()
{    
    if (tcpSocket){
        QByteArray data = tcpSocket->readAll();
        if (handleReceiveDataCheckBox->isChecked()){
            emit bytesRead(data);;
        }
    }
}

void SAKOtherTransmissionItemTcp::setUiEnable(bool enable)
{
    customAddressCheckBox->setEnabled(enable);
    localAddressComboBox->setEnabled(enable);
    localPortLineEdit->setEnabled(enable);
    serverAddressLineEdit->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}
