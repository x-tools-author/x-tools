/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsVelometerTool.h"

#include <QTimer>

xToolsVelometerTool::xToolsVelometerTool(QObject *parent)
    : xToolsBaseTool{parent}
{}

void xToolsVelometerTool::inputBytes(const QByteArray &bytes)
{
    if (isRunning()) {
        mInputBytesContextListMutex.lock();
        mInputBytesContextList.append(bytes);
        mInputBytesContextListMutex.unlock();
    }
}

void xToolsVelometerTool::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        this->mInputBytesContextListMutex.lock();
        auto list = this->mInputBytesContextList;
        this->mInputBytesContextList.clear();
        this->mInputBytesContextListMutex.unlock();

        int v = 0;
        for (auto &bytes : list) {
            v += bytes.length();
        }

        QString cookedVelocity;
        if (v < 1024) {
            cookedVelocity = QString("%1Bytes/s").arg(v);
        } else if (v < 1024 * 1024) {
            cookedVelocity = QString("%1KB/s").arg(v / 1024);
        } else {
            cookedVelocity = QString("%1MB/s").arg(v / (1024 * 1024));
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

QString xToolsVelometerTool::velocity()
{
    mVelocityMutex.lock();
    QString v = mVelocity;
    mVelocityMutex.lock();
    return v;
}
