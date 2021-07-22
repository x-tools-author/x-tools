/******************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>

#include "SAKCommonCrcInterface.hh"
#include "SAKInputCrcSettingsDialog.hh"
#include "ui_SAKInputCrcSettingsDialog.h"

SAKInputCrcSettingsDialog::SAKInputCrcSettingsDialog(QString settingsGroup,
                                                     QSettings *settings,
                                                     QWidget *parent)
    :QDialog(parent)
    ,mSettings(settings)
    ,mUi(new Ui::SAKInputCrcSettingsDialog)
{
    // Initialize variable.
    mUi->setupUi(this);


    QString group = settingsGroup + "/crc/";
    mSettingsKeyContext.append = group + "append";
    mSettingsKeyContext.bigEndian = group + "bigEndian";
    mSettingsKeyContext.parameterMoldel = group + "prameterMoldel";
    mSettingsKeyContext.startByte = group + "startByte";
    mSettingsKeyContext.endByte = group + "endByte";


    auto blockUiSignals = [=](bool block){
        mUi->crcParametersModelComboBox->blockSignals(block);
        mUi->appendCrcCheckBox->blockSignals(block);
        mUi->bigEndianCheckBox->blockSignals(block);
        mUi->startSpinBox->blockSignals(block);
        mUi->endSpinBox->blockSignals(block);
        blockSignals(block);
    };


    blockUiSignals(true);
    auto cb = mUi->crcParametersModelComboBox;
    SAKCommonCrcInterface::addCrcModelItemsToComboBox(cb);


    // Readin settings parameters.
    int index = mSettings->value(mSettingsKeyContext.parameterMoldel).toInt();
    cb->setCurrentIndex(index);
    bool checked = mSettings->value(mSettingsKeyContext.append).toBool();
    mUi->appendCrcCheckBox->setChecked(checked);
    checked = mSettings->value(mSettingsKeyContext.bigEndian).toBool();
    mUi->bigEndianCheckBox->setChecked(checked);
    auto startByte = mSettings->value(mSettingsKeyContext.startByte).toInt();
    mUi->startSpinBox->setValue(startByte < 1 ? 1 : startByte);
    auto endByte = mSettings->value(mSettingsKeyContext.endByte).toInt();
    mUi->endSpinBox->setValue(endByte < 1 ? 1 : endByte);


    // Initialize parameters.
    int model = mUi->crcParametersModelComboBox->currentData().toInt();
    mParametersContext.parameterMoldel = model;
    mParametersContext.append = mUi->appendCrcCheckBox->isChecked();
    mParametersContext.bigEndian = mUi->bigEndianCheckBox->isChecked();
    mParametersContext.startByte = mUi->startSpinBox->value();
    mParametersContext.endByte = mUi->endSpinBox->value();


    // Signals and slots
    connect(mUi->bigEndianCheckBox, &QCheckBox::clicked, this, [=](){
        mParametersContextMutex.lock();
        mParametersContext.bigEndian = mUi->bigEndianCheckBox->isChecked();
        mParametersContextMutex.unlock();

        mSettings->setValue(mSettingsKeyContext.bigEndian,
                             mUi->bigEndianCheckBox->isChecked());
        emit crcParametersChanged();
    });
    connect(mUi->startSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int value){
        mParametersContextMutex.lock();
        mParametersContext.startByte = value;
        mParametersContextMutex.unlock();

        mSettings->setValue(mSettingsKeyContext.startByte,
                            mUi->startSpinBox->value());
        emit crcParametersChanged();
    });
    connect(mUi->endSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int value){
        mParametersContextMutex.lock();
        mParametersContext.endByte = value;
        mParametersContextMutex.unlock();

        mSettings->setValue(mSettingsKeyContext.endByte,
                            mUi->endSpinBox->value());
        emit crcParametersChanged();
    });
    connect(mUi->crcParametersModelComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](){
        mParametersContextMutex.lock();
        int m = mUi->crcParametersModelComboBox->currentData().toInt();
        mParametersContext.parameterMoldel = m;
        mParametersContextMutex.unlock();

        mSettings->setValue(mSettingsKeyContext.parameterMoldel,
                            mUi->crcParametersModelComboBox->currentIndex());
        emit crcParametersChanged();
    });
    connect(mUi->appendCrcCheckBox, &QCheckBox::clicked,
            this, [=](){
        mParametersContextMutex.lock();
        mParametersContext.append = mUi->appendCrcCheckBox->isChecked();
        mParametersContextMutex.unlock();

        mSettings->setValue(mSettingsKeyContext.append,
                            mUi->appendCrcCheckBox->isChecked());
        emit crcParametersChanged();
    });


    setModal(true);
    setWindowTitle(tr("CRC Parameters Configuration"));
    blockUiSignals(false);
}

SAKInputCrcSettingsDialog::~SAKInputCrcSettingsDialog()
{
    delete mUi;
}

SAKInputCrcSettingsDialog::SAKStructCrcParametersContext
SAKInputCrcSettingsDialog::parametersContext()
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
