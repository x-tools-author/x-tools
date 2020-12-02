/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include "SAKDebugPage.hh"
#include "SAKDebugPageDeviceMask.hh"
#include "ui_SAKDebugPageDeviceMask.h"

SAKDebugPageDeviceMask::SAKDebugPageDeviceMask(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,ui(new Ui::SAKDebugPageDeviceMask)
{
    ui->setupUi(this);
    mParametersContext.enableMask = false;

    mSettings = mDebugPage->settings();
    mSettingsKeyRxMask = QString("%1/rxMask").arg(mDebugPage->settingsGroup());
    mSettingsKeyTxMask = QString("%1/txMask").arg(mDebugPage->settingsGroup());

    ui->rxMaskSpinBox->blockSignals(true);
    ui->rxMaskSpinBox->setValue(mSettings->value(mSettingsKeyRxMask).toInt());
    ui->rxMaskSpinBox->blockSignals(false);

    ui->txMaskSpinBox->blockSignals(true);
    ui->txMaskSpinBox->setValue(mSettings->value(mSettingsKeyTxMask).toInt());
    ui->txMaskSpinBox->blockSignals(false);
}

SAKDebugPageDeviceMask::~SAKDebugPageDeviceMask()
{
    delete ui;
}

SAKDebugPageDeviceMask::ParametersContext SAKDebugPageDeviceMask::parametersContext()
{
    mParametersContextMutex.lock();
    auto parasCtx = mParametersContext;
    mParametersContextMutex.unlock();
    return parasCtx;
}

void SAKDebugPageDeviceMask::on_rxMaskSpinBox_valueChanged(int arg1)
{
    mParametersContextMutex.lock();
    mParametersContext.rxMask = arg1;
    mParametersContextMutex.unlock();
    mSettings->setValue(mSettingsKeyRxMask, arg1);
}

void SAKDebugPageDeviceMask::on_txMaskSpinBox_valueChanged(int arg1)
{
    mParametersContextMutex.lock();
    mParametersContext.txMask = arg1;
    mParametersContextMutex.unlock();
    mSettings->setValue(mSettingsKeyTxMask, arg1);
}

void SAKDebugPageDeviceMask::on_enableMaskCheckBox_clicked()
{
    mParametersContextMutex.lock();
    mParametersContext.enableMask = ui->enableMaskCheckBox->isChecked();
    mParametersContextMutex.unlock();
}
