/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>

#include "SAKInterface.h"
#include "SAKMaskerTool.h"

SAKMaskerTool::SAKMaskerTool(QObject *parent)
    : SAKBaseTool{"sak.masktool", parent}
{
    mEnable = false;
}

SAKMaskerTool::~SAKMaskerTool()
{
    qCDebug(mLoggingCategory) << __FUNCTION__;
}

void SAKMaskerTool::setMaskCode(qint8 maskCode)
{
    mMask = maskCode;
}

void SAKMaskerTool::inputBytes(const QByteArray &bytes,
                               const QVariant &context)
{
    emit bytesInputted(bytes, context);

    mInputParametersListMutex.lock();
    InputParameters params;
    params.bytes = bytes;
    params.context = context;
    mInputParametersList.append(params);
    mInputParametersListMutex.unlock();
}

void SAKMaskerTool::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(5);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=](){
        QByteArray bytes;
        QVariant context;
        this->mInputParametersListMutex.lock();
        if (!this->mInputParametersList.isEmpty()) {
            auto ctx = mInputParametersList.takeFirst();
            bytes = ctx.bytes;
            context = ctx.context;
        }
        this->mInputParametersListMutex.unlock();

        if (!bytes.isEmpty()) {
            QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
            QString hex = QString::fromLatin1(ba);
            outputMessage(QtInfoMsg,
                          QString("%1<-%2").arg(mToolName, hex));

            if (this->enable()) {
                QByteArray cookedBytes;
                for (int i = 0; i < bytes.length(); i++) {
                    quint8 value = quint8(bytes.at(i));
                    value ^= mMask;
                    cookedBytes.append(reinterpret_cast<char*>(&value), 1);
                }

                ba = SAKInterface::arrayToHex(cookedBytes, ' ');
                QString hex = QString::fromLatin1(ba);
                outputMessage(QtInfoMsg,
                              QString("%1->%2").arg(mToolName, hex));
                emit bytesOutputted(cookedBytes, context);
            } else {
                ba = SAKInterface::arrayToHex(bytes, ' ');
                QString hex = QString::fromLatin1(ba);
                outputMessage(QtInfoMsg,
                              QString("%1->%2").arg(mToolName, hex));
                emit bytesOutputted(bytes, context);
            }
        }

        timer->start();
    });

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}
