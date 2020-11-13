/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QLabel>
#include <QDebug>
#include <QSizePolicy>
#include <QMessageBox>
#include <QModbusServer>

#include "SAKModbusCommonController.hh"

SAKModbusCommonController::SAKModbusCommonController(QWidget *parent)
    :QWidget(parent)
    ,mBottomSection(Q_NULLPTR)
    ,mDevice(Q_NULLPTR)
{
    mSectionLayout = new QVBoxLayout(this);
    mSectionLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
    setLayout(mSectionLayout);
}

void SAKModbusCommonController::closeDevice()
{
    device()->disconnectDevice();
}

void SAKModbusCommonController::appendSection(QWidget *section)
{
    Q_ASSERT_X(section, __FUNCTION__, "Parameter can not be null!");
    mSectionLayout->addWidget(section);
    if (bottomSection() && mSectionLayout){
        mSectionLayout->addWidget(bottomSection());
    }
}

QModbusDevice *SAKModbusCommonController::device()
{
    return mDevice;
}

void SAKModbusCommonController::init()
{
    mDevice = initModbusDevice();
    mDevice->setParent(this);
    Q_ASSERT_X(mDevice, __FUNCTION__, "The value returned can not be null!");

    auto server = qobject_cast<QModbusServer*>(mDevice);
    if (server){
        QModbusDataUnitMap reg;
        reg.insert(QModbusDataUnit::Coils, {QModbusDataUnit::Coils, 0, 65535});
        reg.insert(QModbusDataUnit::DiscreteInputs, {QModbusDataUnit::DiscreteInputs, 0, 65535});
        reg.insert(QModbusDataUnit::InputRegisters, {QModbusDataUnit::InputRegisters, 0, 65535});
        reg.insert(QModbusDataUnit::HoldingRegisters, {QModbusDataUnit::HoldingRegisters, 0, 65535});
        server->setMap(reg);
        for (int i = 0; i < 65535; i++){
            server->setData(QModbusDataUnit::Coils, i, false);
            server->setData(QModbusDataUnit::DiscreteInputs, i, false);
            server->setData(QModbusDataUnit::InputRegisters, i, 0xffff);
            server->setData(QModbusDataUnit::HoldingRegisters, i, 0xffff);
        }
    }

    if (mDevice){
        connect(mDevice, &QModbusDevice::errorOccurred, this, [=](){
            QMessageBox::warning(this, tr("Error Occurred"), tr("Error Occured:%1 Please check the parameters and try again!").arg(mDevice->errorString()));
        });
    }
}

QWidget *SAKModbusCommonController::bottomSection()
{
    return Q_NULLPTR;
}

QModbusDevice *SAKModbusCommonController::initModbusDevice()
{
    return Q_NULLPTR;
}
