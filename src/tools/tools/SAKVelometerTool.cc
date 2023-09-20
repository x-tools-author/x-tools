/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QTimer>
#include "SAKVelometerTool.h"

SAKVelometerTool::SAKVelometerTool(QObject *parent)
    : SAKBaseTool{"sak.velometertool", parent}
{

}

void SAKVelometerTool::inputBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    if (isRunning()) {
        InputBytesContext ctx;
        ctx.bytes = bytes;
        ctx.context = context;

        mInputBytesContextListMutex.lock();
        mInputBytesContextList.append(ctx);
        mInputBytesContextListMutex.unlock();
    }
}

void SAKVelometerTool::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=](){
        this->mInputBytesContextListMutex.lock();
        auto list = this->mInputBytesContextList;
        this->mInputBytesContextList.clear();
        this->mInputBytesContextListMutex.unlock();

        int v = 0;
        for (auto &ctx : list) {
            v += ctx.bytes.length();
        }

        QString cookedVelocity;
        if (v < 1024) {
            cookedVelocity = QString("%1Bytes/s").arg(v);
        } else if (v < 1024*1024) {
            cookedVelocity = QString("%1KB/s").arg(v/1024);
        } else {
            cookedVelocity = QString("%1MB/s").arg(v/(1024*1024));
        }

        this->mVelocityMutex.lock();
        this->mVelocity = cookedVelocity;
        this->mVelocityMutex.unlock();

        emit velocityChanged(cookedVelocity);
        timer->start();
    });

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}

QString SAKVelometerTool::velocity()
{
    mVelocityMutex.lock();
    QString v = mVelocity;
    mVelocityMutex.lock();
    return v;
}
