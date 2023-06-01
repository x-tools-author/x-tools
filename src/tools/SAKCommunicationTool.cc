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
    QJsonObject jsonObj = context.toJsonObject();
    jsonObj.insert("flag", "tx");
    QVariant cookedContext = QVariant::fromValue(jsonObj);
    InputDataContext dataCtx{bytes, cookedContext};
    mInputDataList.append(dataCtx);
    mInputDataMutex.unlock();;
}

void SAKCommunicationTool::run()
{
    if (!initialize()) {
        outputMessage(QtWarningMsg, "Can not initialize the communication tool!");
        return;
    }

    QTimer *txTimer = new QTimer();
    txTimer->setInterval(5);
    txTimer->setSingleShot(true);
    connect(txTimer, &QTimer::timeout, txTimer, [=](){
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
