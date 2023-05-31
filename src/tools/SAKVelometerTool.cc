/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include "SAKVelometerTool.hh"

SAKVelometerTool::SAKVelometerTool(QObject *parent)
    : SAKBaseTool{"SAK.VelometerTool", parent}
{

}

void SAKVelometerTool::inputBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    if (isRunning()) {
        mInputBytesContextListMutex.lock();
        mInputBytesContextList.append({bytes, context});
        mInputBytesContextListMutex.unlock();
    }
}

void SAKVelometerTool::run()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=](){
        this->mInputBytesContextListMutex.lock();
        auto list = this->mInputBytesContextList
        this->mInputBytesContextListMutex.unlock();

        int rxV = 0;
        int txV = 0;
        for (auto &ctx : list) {
            ctx.
        }
    });

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}

QString SAKVelometerTool::rxV()
{
    mRxVMutex.lock();
    QString v = mRxV;
    mRxVMutex.lock();
    return v;
}

QString SAKVelometerTool::txV()
{
    mTxVMutex.lock();
    QString v = mTxV;
    mTxVMutex.lock();
    return v;
}
