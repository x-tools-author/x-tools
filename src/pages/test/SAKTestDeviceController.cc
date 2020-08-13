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
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKTestDeviceController.hh"
#include "ui_SAKTestDeviceController.h"
SAKTestDeviceController::SAKTestDeviceController(QWidget *parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKTestDeviceController)
{
    mUi->setupUi(this);
    mParameters.openFailed = false;
    mParameters.readCyclic = false;
    mParameters.readInterval = 500;
    mParameters.writeCyclic = false;
    mParameters.writtingInterval = 500;
}

SAKTestDeviceController::~SAKTestDeviceController()
{
    delete mUi;
}

SAKTestDeviceController::ParametersContext SAKTestDeviceController::parameters()
{
    ParametersContext paras;
    mParametersMutex.lock();
    paras.openFailed = mParameters.openFailed;
    paras.readCyclic = mParameters.readCyclic;
    paras.errorString = mParameters.errorString;
    paras.writeCyclic = mParameters.writeCyclic;
    paras.readInterval = mParameters.readInterval;
    paras.writtingInterval = mParameters.writtingInterval;
    mParametersMutex.unlock();

    return paras;
}

void SAKTestDeviceController::on_openFailedCheckBox_clicked()
{
    mParametersMutex.lock();
    mParameters.openFailed = mUi->openFailedCheckBox->isChecked();
    mParametersMutex.unlock();
}

void SAKTestDeviceController::on_errorStringLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.errorString = arg1;
    mParametersMutex.unlock();
}

void SAKTestDeviceController::on_readCyclicCheckBox_clicked()
{
    mParametersMutex.lock();
    mParameters.readCyclic = mUi->readCyclicCheckBox->isChecked();
    mParametersMutex.unlock();
}

void SAKTestDeviceController::on_readIntervalLineEdit_textChanged(const QString &arg1)
{
    int interval = arg1.toInt();
    if (interval < 10){
        interval = 10;
    }

    mParametersMutex.lock();
    mParameters.readInterval = interval;
    mParametersMutex.unlock();
}

void SAKTestDeviceController::on_writeCyclicCheckBox_clicked()
{
    mParametersMutex.lock();
    mParameters.writeCyclic = mUi->writeCyclicCheckBox->isChecked();
    mParametersMutex.unlock();
}

void SAKTestDeviceController::on_writtenIntervalLineEdit_textChanged(const QString &arg1)
{
    int interval = arg1.toInt();
    if (interval < 10){
        interval = 10;
    }

    mParametersMutex.lock();
    mParameters.writtingInterval = interval;
    mParametersMutex.unlock();
}
