/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKCommonInterface.hh"
#include "SAKSerialPortController.hh"
#include "ui_SAKSerialPortController.h"

SAKSerialPortController::SAKSerialPortController(QWidget *parent)
    :SAKDebuggerController( parent)
    ,mUi(new Ui::SAKSerialPortController)
{
    mUi->setupUi(this);
    connect(mUi->serialportsComboBox, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->baudrateComboBox, &QComboBox::currentTextChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->databitsComboBox, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->stopbitsComboBox, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->parityComboBox, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->flowControlComboBox, &QComboBox::currentIndexChanged,
            this, &SAKSerialPortController::parametersChanged);
    connect(mUi->customBaudrateCheckBox, &QCheckBox::clicked,
            this, [=](){
        mUi->baudrateComboBox->setEditable(mUi->customBaudrateCheckBox->isChecked());
    });

    refreshDevice();
}

SAKSerialPortController::~SAKSerialPortController()
{
    delete mUi;
}

void SAKSerialPortController::updateUiState(bool opened)
{
    mUi->serialportsComboBox->setEnabled(!opened);
    mUi->baudrateComboBox->setEnabled(!opened);
    mUi->databitsComboBox->setEnabled(!opened);
    mUi->stopbitsComboBox->setEnabled(!opened);
    mUi->parityComboBox->setEnabled(!opened);
    mUi->customBaudrateCheckBox->setEnabled(!opened);
    mUi->flowControlComboBox->setEnabled(!opened);
}

void SAKSerialPortController::refreshDevice()
{
    SAKCommonInterface::addSerialPortNametItemsToComboBox(mUi->serialportsComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(mUi->baudrateComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(mUi->databitsComboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(mUi->stopbitsComboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(mUi->parityComboBox);
    SAKCommonInterface::addSerialPortFlowControlItemsToComboBox(
                mUi->flowControlComboBox
                );
}

SAKCommonDataStructure::SAKStructSerialPortParametersContext
SAKSerialPortController::parametersContext()
{
    SAKCommonDataStructure::SAKStructSerialPortParametersContext ctx;
    ctx.baudRate = mUi->baudrateComboBox->currentText().toInt();

    int databits = mUi->databitsComboBox->currentData().toInt();
    auto cookedDataBits = static_cast<QSerialPort::DataBits>(databits);
    ctx.dataBits = cookedDataBits;

    int flowControl = mUi->flowControlComboBox->currentData().toInt();
    auto cookedFlowControl = static_cast<QSerialPort::FlowControl>(flowControl);
    ctx.flowControl = cookedFlowControl;

    //ctx.intervalNs =

    int parity = mUi->parityComboBox->currentData().toInt();
    auto cookedParity = static_cast<QSerialPort::Parity>(parity);
    ctx.parity = cookedParity;

    ctx.portName = mUi->serialportsComboBox->currentText();

    int stopBits = mUi->stopbitsComboBox->currentData().toInt();
    auto cookedStopBits = static_cast<QSerialPort::StopBits>(stopBits);
    ctx.stopBits = cookedStopBits;

    return ctx;
}
