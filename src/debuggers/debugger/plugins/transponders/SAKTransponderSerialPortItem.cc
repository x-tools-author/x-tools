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
#include <QSerialPortInfo>

#include "SAKCommonInterface.hh"
#include "SAKTransponderSerialPortItem.hh"
#include "ui_SAKTransponderSerialPortItem.h"

SAKTransponderSerialPortItem::SAKTransponderSerialPortItem(QWidget *parent)
    :SAKDebuggerPluginTransponderBase(parent)
    ,mUi(new Ui::SAKTransponderSerialPortItem)
{    
    mUi->setupUi(this);
    initUiComponents();
    initSignals();
}

SAKTransponderSerialPortItem::SAKTransponderSerialPortItem(
        SAKCommonDataStructure::SAKStructSerialPortParametersContext parasCtx,
        QWidget *parent
        )
    :SAKDebuggerPluginTransponderBase(parent)
    ,mUi(new Ui::SAKTransponderSerialPortItem)
{
    mUi->setupUi(this);
    initUiComponents();

    auto setIndexFromText = [](const QString &text, QComboBox *comboBox){
        int index = comboBox->findText(text);
        if (index != -1) {
            comboBox->setCurrentIndex(index);
        }
    };

    auto setIndexFromData = [](const QVariant &data, QComboBox *comboBox){
        int index = comboBox->findData(data);
        if (index != -1) {
            comboBox->setCurrentIndex(index);
        }
    };

    setIndexFromText(parasCtx.portName, mUi->portNameComboBox);
    setIndexFromText(QString::number(parasCtx.baudRate), mUi->baudRateComboBox);

    setIndexFromData(parasCtx.dataBits, mUi->dataBitscomboBox);
    setIndexFromData(parasCtx.parity, mUi->parityComboBox);
    setIndexFromData(parasCtx.stopBits, mUi->stopBitscomboBox);
    setIndexFromData(parasCtx.flowControl, mUi->flowControlComboBox);
    mUi->frameIntervalSpinBox->setValue(parasCtx.frameIntervel);

    initSignals();
}

SAKTransponderSerialPortItem::~SAKTransponderSerialPortItem()
{
    delete mUi;
}

void SAKTransponderSerialPortItem::initUiComponents()
{
    SAKCommonInterface::addSerialPortNametItemsToComboBox(mUi->portNameComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(mUi->baudRateComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(mUi->dataBitscomboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(mUi->stopBitscomboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(mUi->parityComboBox);
    SAKCommonInterface::addSerialPortFlowControlItemsToComboBox(
                mUi->flowControlComboBox
                );
}

SAKCommonDataStructure::SAKStructSerialPortParametersContext
SAKTransponderSerialPortItem::parametersContext()
{
    SAKCommonDataStructure::SAKStructSerialPortParametersContext ctx;
    ctx.baudRate = mUi->baudRateComboBox->currentText().toInt();

    int databits = mUi->dataBitscomboBox->currentData().toInt();
    auto cookedDataBits = static_cast<QSerialPort::DataBits>(databits);
    ctx.dataBits = cookedDataBits;

    int flowControl = mUi->flowControlComboBox->currentData().toInt();
    auto cookedFlowControl = static_cast<QSerialPort::FlowControl>(flowControl);
    ctx.flowControl = cookedFlowControl;

    auto interval = mUi->frameIntervalSpinBox->value();
    ctx.frameIntervel = interval;

    int parity = mUi->parityComboBox->currentData().toInt();
    auto cookedParity = static_cast<QSerialPort::Parity>(parity);
    ctx.parity = cookedParity;

    ctx.portName = mUi->portNameComboBox->currentText();

    int stopBits = mUi->stopBitscomboBox->currentData().toInt();
    auto cookedStopBits = static_cast<QSerialPort::StopBits>(stopBits);
    ctx.stopBits = cookedStopBits;

    return ctx;
}

void SAKTransponderSerialPortItem::initSignals()
{
    connect(mUi->portNameComboBox, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        emit this->portNameChanged(text);
    });

    connect(mUi->baudRateComboBox, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        emit this->baudRateChanged(text.toInt());
    });

    connect(mUi->dataBitscomboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int index){
        Q_UNUSED(index);
        emit this->dataBitsChanged(mUi->dataBitscomboBox->currentData().toInt());
    });

    connect(mUi->parityComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int index){
        Q_UNUSED(index);
        emit this->parityChanged(mUi->parityComboBox->currentData().toInt());
    });

    connect(mUi->stopBitscomboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int index){
        Q_UNUSED(index);
        emit this->stopBitsChanged(mUi->stopBitscomboBox->currentData().toInt());
    });

    connect(mUi->flowControlComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int index){
        Q_UNUSED(index);
        emit this->flowControlChanged(mUi->dataBitscomboBox->currentData().toInt());
    });

    connect(mUi->frameIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [&](int value){
        emit this->frameIntervalChanged(value);
    });
}
