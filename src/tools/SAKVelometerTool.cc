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
        auto list = this->mInputBytesContextList;
        this->mInputBytesContextList.clear();
        this->mInputBytesContextListMutex.unlock();

        int rxV = 0;
        int txV = 0;
        for (auto &ctx : list) {
            QJsonObject jsonObj = ctx.context.toJsonObject();
            QString flag = jsonObj.value("flag").toString();
            if (flag == "rx") {
                rxV += ctx.bytes.length();
            } else if (flag == "tx") {
                txV += ctx.bytes.length();
            }
        }

        auto cookedV = [](int v)->QString{
            QString ret;
            if (v < 1024) {
                ret = QString("%1bytes/s").arg(v);
            } else if (v < 1024*1024) {
                ret = QString("%1KB/s").arg(v/1024);
            } else {
                ret = QString("%1MB/s").arg(v/(1024*1024));
            }
            return ret;
        };

        mRxVMutex.lock();
        mRxV = cookedV(rxV);
        mRxVMutex.unlock();
        emit rxVChanged(mRxV);

        mTxVMutex.lock();
        mTxV = cookedV(txV);
        mTxVMutex.unlock();
        emit txVChanged(mTxV);
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
