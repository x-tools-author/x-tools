/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include "SAKGlobal.hh"
#include "UdpTransmissionItemWidget.hh"
#include "ui_UdpTransmissionItemWidget.h"

UdpTransmissionItemWidget::UdpTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :BaseTransmissionItemWidget (debugPage, parent)
    ,ui(new Ui::UdpTransmissionItemWidget)
    ,udpSocket (nullptr)
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

UdpTransmissionItemWidget::~UdpTransmissionItemWidget()
{
    delete ui;
    if (udpSocket){
        delete udpSocket;
    }
}

void UdpTransmissionItemWidget::write(QByteArray data)
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

void UdpTransmissionItemWidget::on_enableCheckBox_clicked()
{
    auto closeDev = [&](){
        disconnect(udpSocket, &QUdpSocket::readyRead, this, &UdpTransmissionItemWidget::read);
        delete udpSocket;
        udpSocket = nullptr;
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
                connect(udpSocket, &QUdpSocket::readyRead, this, &UdpTransmissionItemWidget::read);
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

void UdpTransmissionItemWidget::read()
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

void UdpTransmissionItemWidget::setUiEnable(bool enable)
{
    customAddressCheckBox->setEnabled(enable);
    addressComboBox->setEnabled(enable);
    portLineEdit->setEnabled(enable);
    targetAddressLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
