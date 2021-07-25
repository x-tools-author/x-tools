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

SAKSerialPortController::SAKSerialPortController(QSettings *settings,
                                                 const QString &settingsGroup,
                                                 QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKSerialPortController)
    ,defauleFrameInterval(4)
{
    mUi->setupUi(this);
    mUi->frameIntervalSpinBox->setMinimum(defauleFrameInterval);
    refreshDevice();


    QString controlGroup = settingsGroup + "/controller/";
    mSettingsKeyContext.frameInterval = controlGroup + "frameInterval";
    mSettingsKeyContext.portName = controlGroup + "portName";
    mSettingsKeyContext.usingCustomBaudRate = controlGroup + "usingCustomBaudRate";
    mSettingsKeyContext.baudRate = controlGroup + "baudRate";


    connect(mUi->serialportsComboBox, &QComboBox::currentIndexChanged,
            this, [=](int index){
        Q_UNUSED(index);
        QString portName = mUi->serialportsComboBox->currentText();
        mSettings->setValue(mSettingsKeyContext.portName, portName);
        emit parametersChanged();
    });
    connect(mUi->baudrateComboBox, &QComboBox::currentTextChanged,
            this, [=](const QString &baudRate){
        mSettings->setValue(mSettingsKeyContext.baudRate, baudRate);
        emit parametersChanged();
    });
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
        bool checked = mUi->customBaudrateCheckBox->isChecked();
        mSettings->setValue(mSettingsKeyContext.usingCustomBaudRate, checked);
        mUi->baudrateComboBox->setEditable(mUi->customBaudrateCheckBox->isChecked());
    });
    connect(mUi->frameIntervalSpinBox, &QSpinBox::valueChanged,
            this, [=](int frameInterval){
        mSettings->setValue(mSettingsKeyContext.frameInterval, frameInterval);
        emit parametersChanged();
    });


    int frameInterval = settings->value(mSettingsKeyContext.frameInterval).toInt();
    if (frameInterval < defauleFrameInterval) {
        frameInterval = defauleFrameInterval;
    }
    mUi->frameIntervalSpinBox->setValue(frameInterval);

    QString portName = mSettings->value(mSettingsKeyContext.portName).toString();
    if (!portName.isEmpty()) {
        int index = mUi->serialportsComboBox->findText(portName);
        if (index >= 0) {
            mUi->serialportsComboBox->blockSignals(true);
            mUi->serialportsComboBox->setCurrentIndex(index);
            mUi->serialportsComboBox->blockSignals(false);
        }
    }

    bool usingCustomBaudRate =
            mSettings->value(mSettingsKeyContext.usingCustomBaudRate).toBool();
    mUi->baudrateComboBox->setEditable(usingCustomBaudRate);
    mUi->customBaudrateCheckBox->setChecked(usingCustomBaudRate);

    QString baudRate = mSettings->value(mSettingsKeyContext.baudRate).toString();
    mUi->baudrateComboBox->setCurrentText(baudRate);
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
    mUi->serialportsComboBox->blockSignals(true);
    mUi->baudrateComboBox->blockSignals(true);

    SAKCommonInterface::addSerialPortNametItemsToComboBox(mUi->serialportsComboBox);
    SAKCommonInterface::addSerialPortBaudRateItemsToComboBox(mUi->baudrateComboBox);
    SAKCommonInterface::addSerialPortDataBitItemsToComboBox(mUi->databitsComboBox);
    SAKCommonInterface::addSerialPortStopBitItemsToComboBox(mUi->stopbitsComboBox);
    SAKCommonInterface::addSerialPortParityItemsToComboBox(mUi->parityComboBox);
    SAKCommonInterface::addSerialPortFlowControlItemsToComboBox(
                mUi->flowControlComboBox
                );

    mUi->serialportsComboBox->blockSignals(false);
    mUi->baudrateComboBox->blockSignals(false);
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

    // Calculate the max frame interval.
    qint64 consumeNsPerBit = (1000*1000*1000)/ctx.baudRate;
    auto interval = mUi->frameIntervalSpinBox->value();
    if (interval < defauleFrameInterval) {
        ctx.intervalNs = defauleFrameInterval*consumeNsPerBit;
    } else {
        ctx.intervalNs = interval*consumeNsPerBit;
    }
    ctx.intervalNs = mUi->frameIntervalSpinBox->value();

    int parity = mUi->parityComboBox->currentData().toInt();
    auto cookedParity = static_cast<QSerialPort::Parity>(parity);
    ctx.parity = cookedParity;

    ctx.portName = mUi->serialportsComboBox->currentText();

    int stopBits = mUi->stopbitsComboBox->currentData().toInt();
    auto cookedStopBits = static_cast<QSerialPort::StopBits>(stopBits);
    ctx.stopBits = cookedStopBits;

    return ctx;
}
