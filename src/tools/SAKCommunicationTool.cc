/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include "SAKCommunicationTool.hh"

SAKCommunicationTool::SAKCommunicationTool(const char *logCategory,
                                           QObject *parent)
    : SAKBaseTool{logCategory, parent}
{

}

void SAKCommunicationTool::inputBytes(const QByteArray &bytes,
                                      const QVariant &context)
{
    mInputDataMutex.lock();
    DataContext dataCtx{bytes, context};
    mInputDataList.append(dataCtx);
    mInputDataMutex.unlock();;
}

void SAKCommunicationTool::run()
{
    if (!initialize()) {
        return;
    }

    QTimer *rxTimer = new QTimer();
    rxTimer->setInterval(5);
    rxTimer->setSingleShot(true);
    connect(rxTimer, &QTimer::timeout, rxTimer, [=](){
        if (mIsTimerReading) {
            readBytes();
        }
        rxTimer->start();
    });
    rxTimer->start();

    QTimer *txTimer = new QTimer();
    txTimer->setInterval(5);
    txTimer->setSingleShot(true);
    connect(txTimer, &QTimer::timeout, txTimer, [=](){
        this->mInputDataMutex.lock();
        if (!mInputDataList.isEmpty()) {
            DataContext dataCtx = mInputDataList.takeFirst();
            writeBytes(dataCtx.bytes, dataCtx.context);
        }
        this->mInputDataMutex.unlock();
        txTimer->start();
    });
    txTimer->start();

    exec();
    rxTimer->deleteLater();
    rxTimer = nullptr;
    uninitialize();
}
