/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
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


    QString group = settingsGroup + "/crc/";
    mSettingsKeyContext.append = group + "append";
    mSettingsKeyContext.bigEndian = group + "bigEndian";
    mSettingsKeyContext.parameterMoldel = group + "prameterMoldel";
    mSettingsKeyContext.startByte = group + "startByte";
    mSettingsKeyContext.endByte = group + "endByte";


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
    mCrcParametersModelComboBox->setCurrentIndex(mSettings->value(mSettingsKeyContext.parameterMoldel).toInt());
    mAppendCrcCheckBox->setChecked(mSettings->value(mSettingsKeyContext.append).toBool());
    mBigEndianCheckBox->setChecked(mSettings->value(mSettingsKeyContext.bigEndian).toBool());
    auto startByte = mSettings->value(mSettingsKeyContext.startByte).toInt();
    mStartSpinBox->setValue(startByte < 1 ? 1 : startByte);
    auto endByte = mSettings->value(mSettingsKeyContext.endByte).toInt();
    mEndSpinBox->setValue(endByte < 1 ? 1 : endByte);


    // Initialize parameters.
    mParametersContext.parameterMoldel = mCrcParametersModelComboBox->currentData().toInt();
    mParametersContext.append = mAppendCrcCheckBox->isChecked();
    mParametersContext.bigEndian = mBigEndianCheckBox->isChecked();
    mParametersContext.startByte = mStartSpinBox->value();
    mParametersContext.endByte = mEndSpinBox->value();


    setModal(true);
    setWindowTitle(tr("CRC Parameters Configuration"));
    blockUiSignals(false);
}

SAKInputCrcSettingsDialog::~SAKInputCrcSettingsDialog()
{
    delete mUi;
}

SAKInputCrcSettingsDialog::SAKStructCrcParametersContext SAKInputCrcSettingsDialog::parametersContext()
{
    SAKInputCrcSettingsDialog::SAKStructCrcParametersContext ctx;
    mParametersContextMutex.lock();
    ctx.parameterMoldel = mParametersContext.parameterMoldel;
    ctx.append = mParametersContext.append;
    ctx.bigEndian = mParametersContext.bigEndian;
    ctx.endByte = mParametersContext.endByte;
    ctx.startByte = mParametersContext.startByte;
    ctx.bigEndian = mParametersContext.bigEndian;
    mParametersContextMutex.unlock();
    return ctx;
}

void SAKInputCrcSettingsDialog::on_bigEndianCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.bigEndian = mBigEndianCheckBox->isChecked();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingsKeyContext.bigEndian, mBigEndianCheckBox->isChecked());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_startSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.startByte = value;
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingsKeyContext.startByte, mStartSpinBox->value());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_endSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.endByte = value;
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingsKeyContext.endByte, mEndSpinBox->value());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_crcParametersModelComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersContextMutex.lock();
    mParametersContext.parameterMoldel = mCrcParametersModelComboBox->currentData().toInt();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingsKeyContext.parameterMoldel, mCrcParametersModelComboBox->currentIndex());
    emit crcParametersChanged();
}

void SAKInputCrcSettingsDialog::on_appendCrcCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.append = mAppendCrcCheckBox->isChecked();
    mParametersContextMutex.unlock();

    mSettings->setValue(mSettingsKeyContext.append, mAppendCrcCheckBox->isChecked());
    emit crcParametersChanged();
}
