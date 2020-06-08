/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKGlobal.hh"
#include "SAKUdpTransmissionItemWidget.hh"

#include "ui_SAKUdpTransmissionItemWidget.h"

SAKUdpTransmissionItemWidget::SAKUdpTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent)
    :SAKBaseTransmissionItemWidget (_debugPage, parent)
    ,ui(new Ui::SAKUdpTransmissionItemWidget)
    ,udpSocket (Q_NULLPTR)
{
    ui->setupUi(this);

    enableCheckBox = ui->enableCheckBox;
    customAddressCheckBox = ui->customAddressCheckBox;
    addressComboBox = ui->addressComboBox;
    portLineEdit = ui->portLineEdit;
    handleReceiveDataCheckBox = ui->handleReceiveDataCheckBox;
    targetAddressLineEdit = ui->targetAddressLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;

    SAKGlobal::initIpComboBox(addressComboBox);
}

SAKUdpTransmissionItemWidget::~SAKUdpTransmissionItemWidget()
{
    delete ui;
    if (udpSocket){
        delete udpSocket;
    }
}

void SAKUdpTransmissionItemWidget::write(QByteArray data)
{
    if (udpSocket){
        QHostAddress targetAddress(targetAddressLineEdit->text());
        quint16 targetPort = static_cast<quint16>(targetPortLineEdit->text().toInt());
        if (!udpSocket->writeDatagram(data, targetAddress, targetPort)){
#ifdef QT_DEBUG
        qDebug() << "发送数据失败" << udpSocket->errorString();
#endif
        }
    }
}

void SAKUdpTransmissionItemWidget::on_enableCheckBox_clicked()
{
    auto closeDev = [&](){
        disconnect(udpSocket, &QUdpSocket::readyRead, this, &SAKUdpTransmissionItemWidget::read);
        delete udpSocket;
        udpSocket = Q_NULLPTR;
        this->setUiEnable(true);
    };

    auto bindDev = [&](QHostAddress address, quint16 port, bool customAddressAndPort){
        bool bindResult = false;
        if (customAddressAndPort){
            bindResult = udpSocket->bind(address, port);
        }else{
            bindResult = udpSocket->bind();
        }

        if (bindResult){
            if (udpSocket->open(QUdpSocket::ReadWrite)){
                connect(udpSocket, &QUdpSocket::readyRead, this, &SAKUdpTransmissionItemWidget::read);
                this->setUiEnable(false);
                return;
            }
        }

        emit requestOutputMessage(udpSocket->errorString(), false);
        enableCheckBox->setChecked(false);
        closeDev();
    };

    if (enableCheckBox->isChecked()){
        udpSocket = new QUdpSocket;
        if (customAddressCheckBox->isChecked()){
            bindDev(QHostAddress(addressComboBox->currentText()), static_cast<quint16>(portLineEdit->text().toInt()), true);
        }else{
            bindDev(QHostAddress(addressComboBox->currentText()), static_cast<quint16>(portLineEdit->text().toInt()), false);
        }
    }else{
        closeDev();
    }
}

void SAKUdpTransmissionItemWidget::read()
{
    if (!handleReceiveDataCheckBox->isChecked()){
        return;
    }

    if (udpSocket){
        while (udpSocket->hasPendingDatagrams()) {
            QByteArray data;
            data.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
            udpSocket->readDatagram(data.data(), data.length());
            emit bytesRead(data);
        }
    }
}

void SAKUdpTransmissionItemWidget::setUiEnable(bool enable)
{
    customAddressCheckBox->setEnabled(enable);
    addressComboBox->setEnabled(enable);
    portLineEdit->setEnabled(enable);
    targetAddressLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
