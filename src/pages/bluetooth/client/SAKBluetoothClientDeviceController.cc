/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKBluetoothClientDeviceController.hh"
#include "ui_SAKBluetoothClientDeviceController.h"

SAKBluetoothClientDeviceController::SAKBluetoothClientDeviceController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKBluetoothClientDeviceController)
{
    mUi->setupUi(this);
    mRemoteDeviceComboBox = mUi->remoteDeviceComboBox;
    connect(mRemoteDeviceComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text){
        Q_UNUSED(text);
        mParameters.deviceInfo = mRemoteDeviceComboBox->currentData().value<QBluetoothDeviceInfo>();
    });

    mBluetoothDeviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent;
    connect(mBluetoothDeviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, [=](QBluetoothDeviceInfo info){
        mRemoteDeviceComboBox->addItem(info.name(), QVariant::fromValue(info));
    });
    connect(mBluetoothDeviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, [=](){
        emit messageChange(tr("Scan bluetooth device finished."), true);
    });

    qRegisterMetaType<SAKBluetoothClientDeviceController::BluetoothClientParameters>("SAKBluetoothClientDeviceController::BluetoothClientParameters");
    refreshDevice();
}

SAKBluetoothClientDeviceController::~SAKBluetoothClientDeviceController()
{
    delete mUi;
}

QVariant SAKBluetoothClientDeviceController::parameters()
{
    BluetoothClientParameters parameters;
    mParametersMutex.lock();
    parameters.deviceInfo = mParameters.deviceInfo;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKBluetoothClientDeviceController::setUiEnable(bool opened)
{
    mRemoteDeviceComboBox->setEnabled(!opened);
}

void SAKBluetoothClientDeviceController::refreshDevice()
{
    if (!mBluetoothDeviceDiscoveryAgent->isActive()) {
        mRemoteDeviceComboBox->clear();
        emit messageChange(tr("Scaning bluetooth device."), true);
        mBluetoothDeviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    }
}
