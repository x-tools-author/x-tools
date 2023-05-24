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
    : SAKBaseTool{"SAK.AnalyzerTool", parent}
{
    mEnable = false;
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

void SAKAnalyzerTool::inputBytes(const QByteArray &bytes,
                                 const QVariant &context)
{
    if (bytes.isEmpty()) {
        outputMessage(QtInfoMsg, "Empty input bytes, "
                                 "the operation will be ignored!");
        return;
    }

    outputMessage(QtInfoMsg,
                  QString("Analyzer<-%1").arg(bytes.toHex(' ')));
    emit bytesInputted(bytes, context);

    if (!enable()) {
        outputMessage(QtInfoMsg,
                      QString("Analyzer->%1").arg(bytes.toHex(' ')));
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
        if (mEnable) {
            mInputtedBytesMutex.lock();
            analyze();
            mInputtedBytesMutex.unlock();
        }
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

        return;
    }

    if (mParameters.fixed) {
        while (mInputtedBytes.length() >= mParameters.frameBytes) {
            QByteArray frame(mInputtedBytes.data(),
                             mParameters.frameBytes);
            mInputtedBytes.remove(0, mParameters.frameBytes);
            outputMessage(QtInfoMsg,
                          QString("Analyzer->%1").arg(frame.toHex(' ')));
            emit bytesOutputted(frame, QJsonObject());
        }

        return;
    }

    if (mParameters.separationMark.isEmpty()) {
        if (!mInputtedBytes.isEmpty()) {
            mInputtedBytes.clear();
            QString msg = QString("Analyzer->%1")
                              .arg(mInputtedBytes.toHex(' '));
            outputMessage(QtInfoMsg, msg);
            emit bytesOutputted(mInputtedBytes, QJsonObject());
        }
    } else {
        auto ret = mInputtedBytes.indexOf(mParameters.separationMark);
        if (ret != -1) {
            int len = ret + mParameters.separationMark.length();
            QByteArray frame(mInputtedBytes.constData(), len);
            mInputtedBytes.remove(0, len);
            QString msg = QString("Analyzer->%1").arg(frame.toHex(' '));
            outputMessage(QtInfoMsg, msg);
            emit bytesOutputted(frame, QJsonObject());
        }
    }
}
