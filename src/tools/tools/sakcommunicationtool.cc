/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakcommunicationtool.h"
#include <QTimer>

SAKCommunicationTool::SAKCommunicationTool(const char *logCategory, QObject *parent)
    : SAKBaseTool{logCategory, parent}
{}

void SAKCommunicationTool::inputBytes(const QByteArray &bytes, const QVariant &context)
{
    if (!enable()) {
        return;
    }

    mInputDataMutex.lock();
    QJsonObject jsonObj = context.toJsonObject();
    jsonObj.insert("flag", "tx");
    QVariant cookedContext = QVariant::fromValue(jsonObj);
    InputDataContext dataCtx{bytes, cookedContext};
    mInputDataList.append(dataCtx);
    mInputDataMutex.unlock();
}

QJsonObject SAKCommunicationTool::rxJsonObject() const
{
    QJsonObject obj;
    obj.insert("flag", "rx");
    return obj;
}

QJsonObject SAKCommunicationTool::txJsonObject() const
{
    QJsonObject obj;
    obj.insert("flag", "tx");
    return obj;
}

void SAKCommunicationTool::run()
{
    QString errStr;
    if (!initialize(errStr)) {
        outputMessage(QtWarningMsg, errStr);
        emit errorOccured(errStr);
        return;
    }

    QTimer *txTimer = new QTimer();
    txTimer->setInterval(5);
    txTimer->setSingleShot(true);
    connect(txTimer, &QTimer::timeout, txTimer, [=]() {
        this->mInputDataMutex.lock();
        if (!mInputDataList.isEmpty()) {
            InputDataContext dataCtx = mInputDataList.takeFirst();
            writeBytes(dataCtx.bytes, dataCtx.context);
        }
        this->mInputDataMutex.unlock();
        txTimer->start();
    });
    txTimer->start();

    exec();
    txTimer->deleteLater();
    txTimer = nullptr;
    uninitialize();
}
