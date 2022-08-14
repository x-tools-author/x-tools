/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>
#include <QEventLoop>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>

#include "SAKCommonDataStructure.hh"
#include "SAKBleCentralController.hh"
#include "ui_SAKBleCentralController.h"

SAKBleCentralController::SAKBleCentralController(QSettings *settings,
                                                 const QString &settingsGroup,
                                                 QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKBleCentralController)
    ,defauleFrameInterval(4)
{
    mUi->setupUi(this);
    refreshDevice();
}

SAKBleCentralController::~SAKBleCentralController()
{
    delete mUi;
}

void SAKBleCentralController::updateUiState(bool opened)
{
    mUi->bleDevicesComboBox->setEnabled(!opened);
    mUi->servicesComboBox->setEnabled(opened);
}

void SAKBleCentralController::refreshDevice()
{
    mUi->bleDevicesComboBox->clear();

    QEventLoop loop;
    QBluetoothDeviceDiscoveryAgent discovery;
    discovery.setLowEnergyDiscoveryTimeout(5*1000);
    connect(&discovery, &QBluetoothDeviceDiscoveryAgent::finished,
            this, [=, &discovery, &loop](){
        auto ret = discovery.discoveredDevices();
        for (QBluetoothDeviceInfo &info : ret) {
            if (info.coreConfigurations() ==
                    QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
                QBluetoothDeviceInfo cookedInfo = info;
                QVariant infoVariant = QVariant::fromValue(cookedInfo);
                mUi->bleDevicesComboBox->addItem(info.name(), infoVariant);
            }
        }
        loop.exit();
    });


    discovery.start();
    loop.exec();
}

QVariant SAKBleCentralController::parametersContext()
{
    SAKBleCentralParametersContext ctx;
    QVariant rawInfo = mUi->bleDevicesComboBox->currentData();
    ctx.info = rawInfo.value<QBluetoothDeviceInfo>();
    ctx.uuid = mUi->servicesComboBox->currentText();

    return QVariant::fromValue(ctx);
}

void SAKBleCentralController::setServices(const QStringList &services)
{
    mUi->servicesComboBox->clear();
    for (const QString &service : services) {
        mUi->servicesComboBox->addItem(service);
    }
}
