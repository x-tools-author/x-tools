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
#include <QHostAddress>
#include "SAKCommonApi.hh"
#include "TcpTransmissionItemWidget.hh"
#include "ui_TcpTransmissionItemWidget.h"

TcpTransmissionItemWidget::TcpTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :BaseTransmissionItemWidget (debugPage, parent)
    ,ui (new Ui::TcpTransmissionItemWidget)
{
    ui->setupUi(this);
    enableCheckBox = ui->enableCheckBox;
    customAddressCheckBox = ui->customAddressCheckBox;
    addressComboBox = ui->addressComboBox;
    portLineEdit = ui->portLineEdit;
    handleReceiveDataCheckBox = ui->handleReceiveDataCheckBox;
    targetAddressLineEdit = ui->targetAddressLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;

    SAKCommonApi::instance()->initIpComboBox(addressComboBox);
}

void TcpTransmissionItemWidget::write(QByteArray data)
{
    if (!tcpSocket->write(data)){
#ifdef QT_DEBUG
        qDebug() << "发送取数据失败" << tcpSocket->errorString();
#endif
    }
}

void TcpTransmissionItemWidget::on_enableCheckBox_clicked()
{
    auto closeDev = [&](QTcpSocket *dev){
        if (dev){
            dev->disconnectFromHost();
            if (dev->state() == QTcpSocket::ConnectedState){
                dev->waitForDisconnected();
            }
            disconnect(dev, &QTcpSocket::readyRead, this, &TcpTransmissionItemWidget::read);
            dev->deleteLater();
            dev = nullptr;
            this->setUiEnable(true);
        }
    };

    if (enableCheckBox->isChecked()){
        tcpSocket = new QTcpSocket;
        if (customAddressCheckBox->isChecked()){
             if (!tcpSocket->bind(QHostAddress(addressComboBox->currentText()), static_cast<quint16>(portLineEdit->text().toInt()))){
                emit requestOutputMessage(tr("设备绑定失败：") + tcpSocket->errorString(), false);
                enableCheckBox->setChecked(false);
                closeDev(tcpSocket);
             }
        }

        if (!tcpSocket->open(QTcpSocket::ReadWrite)){
            emit requestOutputMessage(tr("设备无法打开：") + tcpSocket->errorString(), false);
            enableCheckBox->setChecked(false);
            closeDev(tcpSocket);
        }

        connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpTransmissionItemWidget::read);
        tcpSocket->connectToHost(targetAddressLineEdit->text(), static_cast<quint16>(targetPortLineEdit->text().toInt()));
        if (!tcpSocket->waitForConnected()){
            emit requestOutputMessage(tr("无法连接服务器：") + tcpSocket->errorString(), false);
            enableCheckBox->setChecked(false);            
            closeDev(tcpSocket);
        }
    }else{
        closeDev(tcpSocket);
    }
}

void TcpTransmissionItemWidget::read()
{
    if (tcpSocket){
        QByteArray data = tcpSocket->readAll();
        emit bytesRead(data);
    }
}

void TcpTransmissionItemWidget::setUiEnable(bool enable)
{
    customAddressCheckBox->setEnabled(enable);
    addressComboBox->setEnabled(enable);
    portLineEdit->setEnabled(enable);
    targetAddressLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
