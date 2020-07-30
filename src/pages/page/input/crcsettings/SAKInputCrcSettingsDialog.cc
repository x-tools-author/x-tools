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

#include "SAKInputCrcSettingsDialog.hh"
#include "ui_SAKInputCrcSettingsDialog.h"

SAKInputCrcSettingsDialog::SAKInputCrcSettingsDialog(QWidget *parent)
    :QDialog(parent)
    ,mIsInitializing(true)
    ,mUi(new Ui::SAKInputCrcSettingsDialog)
{
    mUi->setupUi(this);
    mBigEndianCheckBox = mUi->bigEndianCheckBox;
    mStartSpinBox = mUi->startSpinBox;
    mEndSpinBox = mUi->endSpinBox;
    setModal(true);
    setWindowTitle(tr("CRC Parameters Settings"));

    mParametersContext.bigEndianCRC = mBigEndianCheckBox->isChecked();
    mParametersContext.startByte = mStartSpinBox->value();
    mParametersContext.endByte = mEndSpinBox->value();

    mIsInitializing = false;
}

SAKInputCrcSettingsDialog::~SAKInputCrcSettingsDialog()
{
    delete mUi;
}

SAKInputCrcSettingsDialog::ParameterContext SAKInputCrcSettingsDialog::parametersContext()
{
    SAKInputCrcSettingsDialog::ParameterContext ctx;
    mParametersContextMutex.lock();
    ctx.endByte = mParametersContext.endByte;
    ctx.startByte = mParametersContext.startByte;
    ctx.bigEndianCRC = mParametersContext.bigEndianCRC;
    mParametersContextMutex.unlock();
    return ctx;
}

void SAKInputCrcSettingsDialog::setBigEndian(bool bigEndian)
{
    mBigEndianCheckBox->setChecked(bigEndian);
    mParametersContext.bigEndianCRC = bigEndian;
}

void SAKInputCrcSettingsDialog::setStartByte(int startByte)
{
    mStartSpinBox->setValue(startByte > 0 ? startByte : 1);
}

void SAKInputCrcSettingsDialog::setEndByte(int endByte)
{
    mEndSpinBox->setValue(endByte > 0 ? endByte : 1);
}

void SAKInputCrcSettingsDialog::on_bigEndianCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.bigEndianCRC = mBigEndianCheckBox->isChecked();
    mParametersContextMutex.unlock();
    if (!mIsInitializing){
        emit parametersChanged();
    }
}

void SAKInputCrcSettingsDialog::on_startSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.startByte = value;
    mParametersContextMutex.unlock();
    if (!mIsInitializing){
        emit parametersChanged();
    }
}

void SAKInputCrcSettingsDialog::on_endSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.endByte = value;
    mParametersContextMutex.unlock();
    if (!mIsInitializing){
        emit parametersChanged();
    }
}
