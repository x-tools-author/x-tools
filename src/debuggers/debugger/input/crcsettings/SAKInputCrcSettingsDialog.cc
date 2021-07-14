/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QTimer>

#include "SAKCommonCrcInterface.hh"
#include "SAKInputCrcSettingsDialog.hh"
#include "ui_SAKInputCrcSettingsDialog.h"

SAKInputCrcSettingsDialog::SAKInputCrcSettingsDialog(QString settingsGroup, QSettings *settings, QWidget *parent)
    :QDialog(parent)
    ,mSettings(settings)
    ,mUi(new Ui::SAKInputCrcSettingsDialog)
{
    // Initialize variable.
    mUi->setupUi(this);
    mCrcParametersModelComboBox = mUi->crcParametersModelComboBox;
    mAppendCrcCheckBox = mUi->appendCrcCheckBox;
    mBigEndianCheckBox = mUi->bigEndianCheckBox;
    mStartSpinBox = mUi->startSpinBox;
    mEndSpinBox = mUi->endSpinBox;

    mSettingStringAppendCrc = QString("%1/addCRC").arg(settingsGroup);
    mSettingStringBigEndian = QString("%1/bigEndian").arg(settingsGroup);
    mSettingStringCrcParametersModel = QString("%1/parameterModel").arg(settingsGroup);
    mSettingStringStartByte = QString("%1/startByte").arg(settingsGroup);
    mSettingStringEndByte = QString("%1/endByte").arg(settingsGroup);

    auto blockUiSignals = [=](bool block){
        mCrcParametersModelComboBox->blockSignals(block);
        mAppendCrcCheckBox->blockSignals(block);
        mBigEndianCheckBox->blockSignals(block);
        mStartSpinBox->blockSignals(block);
        mEndSpinBox->blockSignals(block);
        blockSignals(block);
    };

    blockUiSignals(true);
    SAKCommonCrcInterface::addCrcModelItemsToComboBox(mCrcParametersModelComboBox);

    // Readin settings parameters.
    mCrcParametersModelComboBox->setCurrentIndex(mSettings->value(mSettingStringCrcParametersModel).toInt());
    mAppendCrcCheckBox->setChecked(mSettings->value(mSettingStringAppendCrc).toBool());
    mBigEndianCheckBox->setChecked(mSettings->value(mSettingStringBigEndian).toBool());
    auto startByte = mSettings->value(mSettingStringStartByte).toInt();
    mStartSpinBox->setValue(startByte < 1 ? 1 : startByte);
    auto endByte = mSettings->value(mSettingStringEndByte).toInt();
    mEndSpinBox->setValue(endByte < 1 ? 1 : endByte);

    // Initialize parameters.
    mParametersContext.crcPrameterMoldel = mCrcParametersModelComboBox->currentData().toInt();
    mParametersContext.appendCrc = mAppendCrcCheckBox->isChecked();
    mParametersContext.bigEndianCrc = mBigEndianCheckBox->isChecked();
    mParametersContext.startByte = mStartSpinBox->value();
    mParametersContext.endByte = mEndSpinBox->value();

    setModal(true);
    setWindowTitle(tr("CRC Parameters Settings"));
    blockUiSignals(false);
}

SAKInputCrcSettingsDialog::~SAKInputCrcSettingsDialog()
{
    delete mUi;
}

SAKInputCrcSettingsDialog::CrcParameterContext SAKInputCrcSettingsDialog::parametersContext()
{
    SAKInputCrcSettingsDialog::CrcParameterContext ctx;
    mParametersContextMutex.lock();
    ctx.crcPrameterMoldel = mParametersContext.crcPrameterMoldel;
    ctx.appendCrc = mParametersContext.appendCrc;
    ctx.bigEndianCrc = mParametersContext.bigEndianCrc;
    ctx.endByte = mParametersContext.endByte;
    ctx.startByte = mParametersContext.startByte;
    ctx.bigEndianCrc = mParametersContext.bigEndianCrc;
    mParametersContextMutex.unlock();
    return ctx;
}

void SAKInputCrcSettingsDialog::on_bigEndianCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.bigEndianCrc = mBigEndianCheckBox->isChecked();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingStringBigEndian, mBigEndianCheckBox->isChecked());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_startSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.startByte = value;
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingStringStartByte, mStartSpinBox->value());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_endSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.endByte = value;
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingStringEndByte, mEndSpinBox->value());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_crcParametersModelComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersContextMutex.lock();
    mParametersContext.crcPrameterMoldel = mCrcParametersModelComboBox->currentData().toInt();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingStringCrcParametersModel, mCrcParametersModelComboBox->currentIndex());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_appendCrcCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.appendCrc = mAppendCrcCheckBox->isChecked();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingStringAppendCrc, mAppendCrcCheckBox->isChecked());
    emit crcParametersChanged();
}
