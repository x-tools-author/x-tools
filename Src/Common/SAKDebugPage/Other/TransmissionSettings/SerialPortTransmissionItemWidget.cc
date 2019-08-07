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
#include "SAKDebugPage.hh"
#include "SAKCommonApi.hh"
#include "SerialPortTransmissionItemWidget.hh"
#include "ui_SerialPortTransmissionItemWidget.h"

#include <QDebug>
#include <QSerialPortInfo>

Q_DECLARE_METATYPE(QSerialPortInfo)

SerialPortTransmissionItemWidget::SerialPortTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :BaseTransmissionItemWidget (debugPage, parent)
    ,ui (new Ui::SerialPortTransmissionItemWidget)
    ,serialPort (nullptr)
{
    ui->setupUi(this);

    enableCheckBox              = ui->enableCheckBox;
    handleReceiveDataCheckBox   = ui->handleReceiveDataCheckBox;
    comComboBox                 = ui->comComboBox;
    customBaudrateCheckBox      = ui->customBaudrateCheckBox;
    baudRateComboBox            = ui->baudRateComboBox;
    dataBitscomboBox            = ui->dataBitscomboBox;
    stopBitscomboBox            = ui->stopBitscomboBox;
    parityComboBox              = ui->parityComboBox;

    SAKCommonApi::instance()->initComComboBox(comComboBox);
    SAKCommonApi::instance()->initBaudRateComboBox(baudRateComboBox);
    SAKCommonApi::instance()->initDataBitsComboBox(dataBitscomboBox);
    SAKCommonApi::instance()->initStopBitsComboBox(stopBitscomboBox);
    SAKCommonApi::instance()->initParityComboBox(parityComboBox);

    handleReceiveDataCheckBox->setChecked(true);
}

SerialPortTransmissionItemWidget::~SerialPortTransmissionItemWidget()
{
    delete ui;
}

void SerialPortTransmissionItemWidget::write(QByteArray data)
{
    if (serialPort){
        if (serialPort->write(data)){
#ifdef QT_DEBUG
            qDebug() << "send data error:" << serialPort->errorString();
#endif
        }
    }
}

void SerialPortTransmissionItemWidget::on_enableCheckBox_clicked()
{
    // c++11 lambda表达式
    auto closeDev = [&](QSerialPort *dev){
        if (dev){
            disconnect(dev, &QSerialPort::readyRead, this, &SerialPortTransmissionItemWidget::read);
            dev->close();
            dev->deleteLater();
            dev = nullptr;
            this->setUiEnable(true);
        }
    };

    if (enableCheckBox->isChecked()){
        serialPort = new QSerialPort(comComboBox->currentData().value<QSerialPortInfo>());
        serialPort->setBaudRate(baudRateComboBox->currentData().value<qint32>());
        serialPort->setParity(parityComboBox->currentData().value<QSerialPort::Parity>());
        serialPort->setDataBits(dataBitscomboBox->currentData().value<QSerialPort::DataBits>());
        serialPort->setStopBits(stopBitscomboBox->currentData().value<QSerialPort::StopBits>());
        if (serialPort->open(QSerialPort::ReadWrite)){
            this->setUiEnable(false);
            connect(serialPort, &QSerialPort::readyRead, this, &SerialPortTransmissionItemWidget::read, Qt::QueuedConnection);
#ifdef QT_DEBUG
            qInfo() << tr("串口打开成功")
                    << tr("串口名称：") << serialPort->portName()
                    << tr("波特率：") << serialPort->baudRate()
                    << tr("数据位：") << serialPort->dataBits()
                    << tr("校验方式：") << serialPort->parity();
#endif
        }else{
            _debugPage->outputMessage(serialPort->errorString(), false);
            enableCheckBox->setChecked(false);
            closeDev(serialPort);
        }
    }else{
        if (serialPort){
            closeDev(serialPort);
        }
    }
}

void SerialPortTransmissionItemWidget::on_customBaudrateCheckBox_clicked()
{
    baudRateComboBox->setEditable(customBaudrateCheckBox->isChecked());
}

void SerialPortTransmissionItemWidget::read()
{
    if (serialPort){
        QByteArray data = serialPort->readAll();
        if (!data.isEmpty()){
            if (handleReceiveDataCheckBox->isChecked()){
                emit bytesRead(data);
            }
        }
    }
}

void SerialPortTransmissionItemWidget::setUiEnable(bool enable)
{
    comComboBox->setEnabled(enable);
    customBaudrateCheckBox->setEnabled(enable);
    baudRateComboBox->setEnabled(enable);
    dataBitscomboBox->setEnabled(enable);
    stopBitscomboBox->setEnabled(enable);
    parityComboBox->setEnabled(enable);
}
