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
    Q_ASSERT_X(mDevice, __FUNCTION__, "The value returned can not be null!");

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
