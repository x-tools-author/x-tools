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
#include "SAKSingletonController.hh"

SAKSingletonController::SAKSingletonController(QObject *parent)
    :QObject(parent)
{
    mSharedMemory = new QSharedMemory(QString("QtSwissArmyKnife"));
    bool ret = mSharedMemory->create(1);
    mSharedMemory->attach();
    mIsInstanceExist = !ret;

    if (ret){
        QTimer *timer = new QTimer(this);
        timer->setInterval(1*1000);
        connect(timer, &QTimer::timeout, this, &SAKSingletonController::checkSharedMemory);
        timer->start();
    }
}

SAKSingletonController::~SAKSingletonController()
{
    if (mSharedMemory->isAttached()){
        mSharedMemory->detach();
    }
}

bool SAKSingletonController::isInstanceExist()
{
    return mIsInstanceExist;
}

void SAKSingletonController::setFlag()
{
    mSharedMemory->lock();
    uint8_t *ptr = reinterpret_cast<uint8_t*>(mSharedMemory->data());
    if (ptr){
        ptr[0] = 1;
    }
    mSharedMemory->unlock();
}

void SAKSingletonController::checkSharedMemory()
{
    mSharedMemory->lock();
    uint8_t *ptr = reinterpret_cast<uint8_t*>(mSharedMemory->data());
    if (ptr){
        if (ptr[0]){
            emit showMainWindowInstanceRequest();
            ptr[0] = 0;
        }
    }
    mSharedMemory->unlock();
}
