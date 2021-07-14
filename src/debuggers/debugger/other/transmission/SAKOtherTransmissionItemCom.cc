/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaType>

#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include <QSerialPortInfo>
Q_DECLARE_METATYPE(QSerialPortInfo)
#endif

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKOtherTransmissionItem.hh"
#include "SAKOtherTransmissionItemCom.hh"

#include "ui_SAKOtherTransmissionItemCom.h"

SAKOtherTransmissionItemCom::SAKOtherTransmissionItemCom(SAKDebugPage *_debugPage, QWidget *parent)
    :SAKOtherTransmissionItem (_debugPage, parent)
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    ,mUi (new Ui::SAKOtherTransmissionItemCom)
    ,mSerialPort (Q_NULLPTR)
#endif
{    
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    mUi->setupUi(this);
    mEnableCheckBox = mUi->enableCheckBox;
    mHandleReceiveDataCheckBox = mUi->handleReceiveDataCheckBox;
    mComComboBox = mUi->comComboBox;
    mCustomBaudrateCheckBox = mUi->customBaudrateCheckBox;
    mBaudRateComboBox = mUi->baudRateComboBox;
    mDataBitscomboBox = mUi->dataBitscomboBox;
    mStopBitscomboBox = mUi->stopBitscomboBox;
    mParityComboBox = mUi->parityComboBox;

    SAKCommonInterface::addSerialPortNametItemsToComboBox(mComComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(mBaudRateComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(mDataBitscomboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(mStopBitscomboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(mParityComboBox);

    mHandleReceiveDataCheckBox->setChecked(true);
#else
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel("该版本软件不支持数据通过串口转发", this);
    label->setObjectName(QString("label%1fkldfjsdajflsadfjasdf").arg(__LINE__));
    label->setStyleSheet(QString("QLabel#%1{color:red}").arg(label->objectName()));
    label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(label, Qt::AlignHCenter);
    setLayout(layout);
#endif
}

#ifdef SAK_IMPORT_MODULE_SERIALPORT
SAKOtherTransmissionItemCom::~SAKOtherTransmissionItemCom()
{
    delete mUi;
}
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
void SAKOtherTransmissionItemCom::write(QByteArray data)
{
    if (mSerialPort){
        if (mSerialPort->write(data)){
#ifdef QT_DEBUG
            qDebug() << "send data error:" << mSerialPort->errorString();
#endif
        }
    }
}
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
void SAKOtherTransmissionItemCom::on_enableCheckBox_clicked()
{
    // c++11 lambda
    auto closeDev = [&](){
        if (mSerialPort){
            disconnect(mSerialPort, &QSerialPort::readyRead, this, &SAKOtherTransmissionItemCom::read);
            mSerialPort->close();
            mSerialPort->deleteLater();
            mSerialPort = Q_NULLPTR;
            this->setUiEnable(true);
        }
    };

    if (mEnableCheckBox->isChecked()){
        mSerialPort = new QSerialPort(mComComboBox->currentText().trimmed());
        mSerialPort->setBaudRate(mBaudRateComboBox->currentData().value<qint32>());
        mSerialPort->setParity(mParityComboBox->currentData().value<QSerialPort::Parity>());
        mSerialPort->setDataBits(mDataBitscomboBox->currentData().value<QSerialPort::DataBits>());
        mSerialPort->setStopBits(mStopBitscomboBox->currentData().value<QSerialPort::StopBits>());
        if (mSerialPort->open(QSerialPort::ReadWrite)){
            this->setUiEnable(false);
            connect(mSerialPort, &QSerialPort::readyRead, this, &SAKOtherTransmissionItemCom::read, Qt::QueuedConnection);
#ifdef QT_DEBUG
            qInfo() << tr("Open device successfully,")
                    << tr("name:") << mSerialPort->portName()
                    << tr("baudrate:") << mSerialPort->baudRate()
                    << tr("data bits:") << mSerialPort->dataBits()
                    << tr("parity:") << mSerialPort->parity();
#endif
        }else{
            emit requestOutputMessage(mSerialPort->errorString(), false);
            mEnableCheckBox->setChecked(false);
            closeDev();
        }
    }else{
        if (mSerialPort){
            closeDev();
        }
    }
}
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
void SAKOtherTransmissionItemCom::on_customBaudrateCheckBox_clicked()
{
    mBaudRateComboBox->setEditable(mCustomBaudrateCheckBox->isChecked());
}
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
void SAKOtherTransmissionItemCom::read()
{
    if (mSerialPort){
        QByteArray data = mSerialPort->readAll();
        if (!data.isEmpty()){
            if (mHandleReceiveDataCheckBox->isChecked()){
                emit bytesRead(data);
            }
        }
    }
}
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
void SAKOtherTransmissionItemCom::setUiEnable(bool enable)
{
    mComComboBox->setEnabled(enable);
    mCustomBaudrateCheckBox->setEnabled(enable);
    mBaudRateComboBox->setEnabled(enable);
    mDataBitscomboBox->setEnabled(enable);
    mStopBitscomboBox->setEnabled(enable);
    mParityComboBox->setEnabled(enable);
}
#endif
