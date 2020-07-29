/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKGlobal.hh"
#include "SAKOtherTransmissionItemUdp.hh"

#include "ui_SAKOtherTransmissionItemUdp.h"

SAKOtherTransmissionItemUdp::SAKOtherTransmissionItemUdp(SAKDebugPage *debugPage, QWidget *parent)
    :SAKOtherTransmissionItem (debugPage, parent)
    ,mUi(new Ui::SAKOtherTransmissionItemUdp)
    ,mUdpSocket (Q_NULLPTR)
{
    mUi->setupUi(this);

    mEnableCheckBox = mUi->enableCheckBox;
    mCustomAddressCheckBox = mUi->customAddressCheckBox;
    mAddressComboBox = mUi->addressComboBox;
    mPortLineEdit = mUi->portLineEdit;
    mHandleReceiveDataCheckBox = mUi->handleReceiveDataCheckBox;
    mTargetAddressLineEdit = mUi->targetAddressLineEdit;
    mTargetPortLineEdit = mUi->targetPortLineEdit;

    SAKGlobal::initIpComboBox(mAddressComboBox);
}

SAKOtherTransmissionItemUdp::~SAKOtherTransmissionItemUdp()
{
    delete mUi;
    if (mUdpSocket){
        delete mUdpSocket;
    }
}

void SAKOtherTransmissionItemUdp::write(QByteArray data)
{
    if (mUdpSocket){
        QHostAddress targetAddress(mTargetAddressLineEdit->text());
        quint16 targetPort = static_cast<quint16>(mTargetPortLineEdit->text().toInt());
        if (!mUdpSocket->writeDatagram(data, targetAddress, targetPort)){
#ifdef QT_DEBUG
        qDebug() << "发送数据失败" << mUdpSocket->errorString();
#endif
        }
    }
}

void SAKOtherTransmissionItemUdp::on_enableCheckBox_clicked()
{
    auto closeDev = [&](){
        disconnect(mUdpSocket, &QUdpSocket::readyRead, this, &SAKOtherTransmissionItemUdp::read);
        delete mUdpSocket;
        mUdpSocket = Q_NULLPTR;
        this->setUiEnable(true);
    };

    auto bindDev = [&](QHostAddress address, quint16 port, bool customAddressAndPort){
        bool bindResult = false;
        if (customAddressAndPort){
            bindResult = mUdpSocket->bind(address, port);
        }else{
            bindResult = mUdpSocket->bind();
        }

        if (bindResult){
            if (mUdpSocket->open(QUdpSocket::ReadWrite)){
                connect(mUdpSocket, &QUdpSocket::readyRead, this, &SAKOtherTransmissionItemUdp::read);
                this->setUiEnable(false);
                return;
            }
        }

        emit requestOutputMessage(mUdpSocket->errorString(), false);
        mEnableCheckBox->setChecked(false);
        closeDev();
    };

    if (mEnableCheckBox->isChecked()){
        mUdpSocket = new QUdpSocket;
        if (mCustomAddressCheckBox->isChecked()){
            bindDev(QHostAddress(mAddressComboBox->currentText()), static_cast<quint16>(mPortLineEdit->text().toInt()), true);
        }else{
            bindDev(QHostAddress(mAddressComboBox->currentText()), static_cast<quint16>(mPortLineEdit->text().toInt()), false);
        }
    }else{
        closeDev();
    }
}

void SAKOtherTransmissionItemUdp::read()
{
    if (!mHandleReceiveDataCheckBox->isChecked()){
        return;
    }

    if (mUdpSocket){
        while (mUdpSocket->hasPendingDatagrams()) {
            QByteArray data;
            data.resize(static_cast<int>(mUdpSocket->pendingDatagramSize()));
            mUdpSocket->readDatagram(data.data(), data.length());
            emit bytesRead(data);
        }
    }
}

void SAKOtherTransmissionItemUdp::setUiEnable(bool enable)
{
    mCustomAddressCheckBox->setEnabled(enable);
    mAddressComboBox->setEnabled(enable);
    mPortLineEdit->setEnabled(enable);
    mTargetAddressLineEdit->setEnabled(enable);
    mTargetPortLineEdit->setEnabled(enable);
}
