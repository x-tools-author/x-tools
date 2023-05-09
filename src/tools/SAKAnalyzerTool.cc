/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKAnalyzerTool.hh"

SAKAnalyzerTool::SAKAnalyzerTool(QObject *parent)
    : SAKBaseTool{"ED.AnalyzerTool", parent}
{

}

void SAKAnalyzerTool::setFixed(bool fixed)
{
    mParameters.fixed = fixed;
}

void SAKAnalyzerTool::setFrameBytes(int bytes)
{
    mParameters.frameBytes = bytes;
}

void SAKAnalyzerTool::setSeparationMark(const QByteArray &mark)
{
    mParameters.separationMark = mark;
}

void SAKAnalyzerTool::setMaxTempBytes(int maxBytes)
{
    mParameters.maxTempBytes = maxBytes;
}

void SAKAnalyzerTool::inputBytes(const QByteArray &bytes, const QVariant &context)
{
    emit bytesInputted(bytes, context);

    if (!enable()) {
        emit bytesOutputted(bytes, context);
    } else {
        mInputtedBytesMutex.lock();
        mInputtedBytes.append(bytes);
        mInputtedBytesMutex.unlock();
    }
}

void SAKAnalyzerTool::run()
{
    QTimer *handleTimer = new QTimer();
    handleTimer->setInterval(5);
    handleTimer->setSingleShot(true);
    connect(handleTimer, &QTimer::timeout, handleTimer, [=](){
        mInputtedBytesMutex.lock();
        analyze();
        mInputtedBytesMutex.unlock();
        handleTimer->start();
    });
    handleTimer->start();

    exec();

    if (handleTimer && handleTimer->isActive()) {
        mInputtedBytesMutex.lock();
        mInputtedBytes.clear();
        mInputtedBytesMutex.unlock();

        handleTimer->stop();
        handleTimer->deleteLater();
    }

    handleTimer = nullptr;
}

void SAKAnalyzerTool::analyze()
{
    if (mInputtedBytes.length() > mParameters.maxTempBytes) {
        outputMessage(QtInfoMsg,
                      "clear bytes: " + QString(mInputtedBytes.toHex(' ')));
        mInputtedBytes.clear();
    } else {
        if (mParameters.fixed) {
            while (mInputtedBytes.length() >= mParameters.frameBytes) {
                QByteArray frame(mInputtedBytes.data(),
                                 mParameters.frameBytes);
                mInputtedBytes.remove(0, mParameters.frameBytes);
                emit bytesOutputted(frame, QJsonObject());
            }
        } else {
            if (mParameters.separationMark.isEmpty()) {
                if (!mInputtedBytes.isEmpty()) {
                    mInputtedBytes.clear();
                    emit bytesOutputted(mInputtedBytes, QJsonObject());
                }
            } else {
                auto ret = mInputtedBytes.indexOf(mParameters.separationMark);
                if (ret != -1) {
                    int len = ret + mParameters.separationMark.length();
                    QByteArray frame(mInputtedBytes.constData(), len);
                    mInputtedBytes.remove(0, len);
                    emit bytesOutputted(frame, QJsonObject());
                }
            }
        }
    }
}
