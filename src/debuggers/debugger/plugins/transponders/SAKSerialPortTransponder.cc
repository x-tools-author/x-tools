/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QMetaType>
#include <QSerialPortInfo>

#include "SAKCommonInterface.hh"
#include "SAKSerialPortTransponder.hh"
#include "ui_SAKSerialPortTransponder.h"

SAKSerialPortTransponder::SAKSerialPortTransponder(QWidget *parent)
    :SAKDebuggerPluginTransponder(parent)
    ,mUi(new Ui::SAKSerialPortTransponder)
    ,mDevice(new SAKSerialPortDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{    
    mUi->setupUi(this);
    initComponents();
    initSignals();
    setupDevice();
}

SAKSerialPortTransponder::SAKSerialPortTransponder(
        quint64 id,
        SAKCommonDataStructure::SAKStructSerialPortParametersContext parasCtx,
        QWidget *parent
        )
    :SAKDebuggerPluginTransponder(id, parent)
    ,mUi(new Ui::SAKSerialPortTransponder)
    ,mDevice(new SAKSerialPortDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    initComponents();

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
    setupDevice();
}

SAKSerialPortTransponder::~SAKSerialPortTransponder()
{
    delete mUi;
    if (mDevice->isRunning()) {
        mDevice->exit();
        mDevice->wait();
    }
}

QVariant SAKSerialPortTransponder::parametersContext()
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

    return QVariant::fromValue(ctx);
}

SAKDebuggerDevice *SAKSerialPortTransponder::device()
{
    return mDevice;
}

void SAKSerialPortTransponder::onDeviceStateChanged(bool opened)
{
    mUi->customBaudrateCheckBox->setEnabled(!opened);
    mUi->portNameComboBox->setEnabled(!opened);
    mUi->baudRateComboBox->setEnabled(!opened);
    mUi->dataBitscomboBox->setEnabled(!opened);
    mUi->stopBitscomboBox->setEnabled(!opened);
    mUi->parityComboBox->setEnabled(!opened);
    mUi->flowControlComboBox->setEnabled(!opened);
    mUi->frameIntervalSpinBox->setEnabled(!opened);
}

void SAKSerialPortTransponder::initComponents()
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

void SAKSerialPortTransponder::initSignals()
{
    connect(mUi->enableCheckBox, &QCheckBox::clicked, this, [=](){
        setEnable(mUi->enableCheckBox->isChecked());
        if (mUi->enableCheckBox->isChecked()) {
            mDevice->start();
        } else {
            mDevice->exit();
            mDevice->wait();
        }
    });
    connect(mUi->customBaudrateCheckBox, &QCheckBox::clicked, this, [=](){
        mUi->baudRateComboBox->setEditable(mUi->customBaudrateCheckBox->isChecked());
    });

    connect(mUi->portNameComboBox, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->portNameChanged(text);
    });

    connect(mUi->baudRateComboBox, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->baudRateChanged(text.toInt());
    });
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->dataBitscomboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mUi->dataBitscomboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [&](int index){
        Q_UNUSED(index);
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->dataBitsChanged(mUi->dataBitscomboBox->currentData().toInt());
    });
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->parityComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mUi->parityComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [&](int index){
        Q_UNUSED(index);
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->parityChanged(mUi->parityComboBox->currentData().toInt());
    });
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->stopBitscomboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mUi->stopBitscomboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [&](int index){
        Q_UNUSED(index);
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->stopBitsChanged(mUi->stopBitscomboBox->currentData().toInt());
    });

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->flowControlComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mUi->flowControlComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [&](int index){
        Q_UNUSED(index);
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->flowControlChanged(mUi->dataBitscomboBox->currentData().toInt());
    });

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->frameIntervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
#else
    connect(mUi->frameIntervalSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
#endif
            this, [&](int value){
        mDevice->setParametersContext(QVariant::fromValue(parametersContext()));
        emit this->frameIntervalChanged(value);
    });

    connect(mDevice, &SAKSerialPortDevice::errorOccurred,
            this, [=]{
        mUi->enableCheckBox->setChecked(false);
    });
}
