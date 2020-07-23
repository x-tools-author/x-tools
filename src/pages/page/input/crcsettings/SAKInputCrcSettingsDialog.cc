/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QTimer>

#include "SAKInputCrcSettingsDialog.hh"
#include "ui_SAKInputCrcSettingsDialog.h"

SAKInputCrcSettingsDialog::SAKInputCrcSettingsDialog(QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKInputCrcSettingsDialog)
{
    mUi->setupUi(this);
    mBigEndianCheckBox = mUi->bigEndianCheckBox;
    mStartSpinBox = mUi->startSpinBox;
    mEndSpinBox = mUi->endSpinBox;
    setModal(true);
    setWindowTitle(tr("CRC参数设置"));

    mParametersContext.bigEndianCRC = mBigEndianCheckBox->isChecked();
    mParametersContext.startByte = mStartSpinBox->value();
    mParametersContext.endByte = mStartSpinBox->value();
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

void SAKInputCrcSettingsDialog::on_bigEndianCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.bigEndianCRC = mBigEndianCheckBox->isChecked();
    mParametersContextMutex.unlock();
    emit parametersChanged();
}

void SAKInputCrcSettingsDialog::on_startSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.startByte = value;
    mParametersContextMutex.unlock();
    emit parametersChanged();
}

void SAKInputCrcSettingsDialog::on_endSpinBox_valueChanged(int value)
{
    mParametersContextMutex.lock();
    mParametersContext.endByte = value;
    mParametersContextMutex.unlock();
    emit parametersChanged();
}
