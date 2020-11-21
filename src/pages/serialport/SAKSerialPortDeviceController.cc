/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKSerialPortDeviceController.hh"
#include "ui_SAKSerialPortDeviceController.h"

SAKSerialPortDeviceController::SAKSerialPortDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mDebugPage(debugPage)
    ,ui(new Ui::SAKSerialPortDeviceController)
    ,serialportsComboBox(Q_NULLPTR)
    ,baudrateComboBox(Q_NULLPTR)
    ,databitsComboBox(Q_NULLPTR)
    ,stopbitsComboBox(Q_NULLPTR)
    ,parityComboBox(Q_NULLPTR)
    ,flowControlComboBox(Q_NULLPTR)
    ,customBaudrateCheckBox(Q_NULLPTR)
{
    ui->setupUi(this);
    serialportsComboBox = ui->serialportsComboBox;
    baudrateComboBox = ui->baudrateComboBox;
    databitsComboBox = ui->databitsComboBox;
    stopbitsComboBox = ui->stopbitsComboBox;
    parityComboBox = ui->parityComboBox;
    flowControlComboBox = ui->flowControlComboBox;
    customBaudrateCheckBox = ui->customBaudrateCheckBox;

    qRegisterMetaType<SAKSerialPortDeviceController::SerialPortParameters>("SAKSerialPortDeviceController::SerialPortParameters");
    refreshDevice();
}

SAKSerialPortDeviceController::~SAKSerialPortDeviceController()
{
    delete ui;
}

QVariant SAKSerialPortDeviceController::parameters()
{
    SerialPortParameters parameters;
    mParametersMutex.lock();
    parameters.name = mParameters.name;
    parameters.parity = mParameters.parity;
    parameters.baudRate = mParameters.baudRate;
    parameters.dataBits = mParameters.dataBits;
    parameters.stopBits = mParameters.stopBits;
    parameters.flowControl = mParameters.flowControl;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKSerialPortDeviceController::setUiEnable(bool opened)
{
    serialportsComboBox->setEnabled(!opened);
    baudrateComboBox->setEnabled(!opened);
    databitsComboBox->setEnabled(!opened);
    stopbitsComboBox->setEnabled(!opened);
    parityComboBox->setEnabled(!opened);
    customBaudrateCheckBox->setEnabled(!opened);
    flowControlComboBox->setEnabled(!opened);
}

void SAKSerialPortDeviceController::refreshDevice()
{
    SAKCommonInterface::addSerialPortNametItemsToComboBox(serialportsComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(baudrateComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(databitsComboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(stopbitsComboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(parityComboBox);
    SAKCommonInterface::addSerialPortFlowControlItemsToComboBox(flowControlComboBox);
}

void SAKSerialPortDeviceController::setBaudRate(quint32 bd)
{
    mParametersMutex.lock();
    // rate can not be zero
    if (!bd){
        bd = 9600;
    }
    mParameters.baudRate = bd;
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_customBaudrateCheckBox_clicked()
{
    if (customBaudrateCheckBox->isChecked()){
        baudrateComboBox->setEditable(true);
        baudrateComboBox->lineEdit()->selectAll();
        baudrateComboBox->lineEdit()->setFocus();
    }else{
        baudrateComboBox->setEditable(false);
    }
}

void SAKSerialPortDeviceController::on_serialportsComboBox_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mParametersMutex.lock();
    mParameters.name = serialportsComboBox->currentText().split(' ').first();
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_baudrateComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    qint32 rate = baudrateComboBox->currentText().toInt();
    setBaudRate(rate);
}

void SAKSerialPortDeviceController::on_databitsComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.dataBits = static_cast<QSerialPort::DataBits>(databitsComboBox->currentData().toInt());
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_stopbitsComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.stopBits = static_cast<QSerialPort::StopBits>(stopbitsComboBox->currentData().toInt());
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_parityComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.parity = static_cast<QSerialPort::Parity>(parityComboBox->currentData().toInt());
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_flowControlComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.flowControl = static_cast<QSerialPort::FlowControl>(flowControlComboBox->currentData().toInt());
    mParametersMutex.unlock();
}

void SAKSerialPortDeviceController::on_baudrateComboBox_editTextChanged(const QString &arg1)
{
    qint32 rate = arg1.toInt();
    setBaudRate(rate);
}
