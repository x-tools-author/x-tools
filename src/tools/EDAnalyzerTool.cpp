/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDAnalyzerTool.hpp"

EDAnalyzerTool::EDAnalyzerTool(QObject *parent)
    : EDBaseTool{"EDAnalyzerTool", parent}
{

}

void EDAnalyzerTool::setFixed(bool fixed)
{
    mParameters.fixed = fixed;
}

void EDAnalyzerTool::setFrameBytes(int bytes)
{
    mParameters.frameBytes = bytes;
}

void EDAnalyzerTool::setSeparationMark(const QByteArray &mark)
{
    mParameters.separationMark = mark;
}

void EDAnalyzerTool::setMaxTempBytes(int maxBytes)
{
    mParameters.maxTempBytes = maxBytes;
}

bool EDAnalyzerTool::initialize(QString &errStr)
{
    mHandleTimer = new QTimer();
    mHandleTimer->setInterval(10);
    mHandleTimer->setSingleShot(true);
    connect(mHandleTimer, &QTimer::timeout, mHandleTimer, [=](){
        analyze();
    });
    mHandleTimer->start();
    return true;
}

void EDAnalyzerTool::inputBytesHandler(const QByteArray &bytes)
{
    if (!enable()) {
        outputBytes(bytes);
    } else {
        mInputtedBytesMutex.lock();
        mInputtedBytes.append(bytes);
        mInputtedBytesMutex.unlock();
    }
}

void EDAnalyzerTool::uninitialize()
{
    if (mHandleTimer && mHandleTimer->isActive()) {
        mInputtedBytesMutex.lock();
        mInputtedBytes.clear();
        mInputtedBytesMutex.unlock();

        mHandleTimer->stop();
        mHandleTimer->deleteLater();
    }

    mHandleTimer = Q_NULLPTR;
}

void EDAnalyzerTool::analyze()
{
    mInputtedBytesMutex.lock();

    if (mInputtedBytes.length() > mParameters.maxTempBytes) {
        outputMessage(QtInfoMsg,
                      "clear bytes: " + QString(mInputtedBytes.toHex(' ')));
        mInputtedBytes.clear();
    } else {
        if (mParameters.fixed) {
            while (mInputtedBytes.length() >= mParameters.frameBytes) {
                QByteArray frame(mInputtedBytes.data(),
                                 mParameters.frameBytes);
                outputBytes(frame);
                mInputtedBytes.remove(0, mParameters.frameBytes);
            }
        } else {
            if (mParameters.separationMark.isEmpty()) {
                if (!mInputtedBytes.isEmpty()) {
                    outputBytes(mInputtedBytes);
                    mInputtedBytes.clear();
                }
            } else {
                auto ret = mInputtedBytes.indexOf(mParameters.separationMark);
                if (ret != -1) {
                    int len = ret + mParameters.separationMark.length();
                    QByteArray frame(mInputtedBytes.constData(), len);
                    outputBytes(frame);
                    mInputtedBytes.remove(0, len);
                }
            }
        }
    }

    mInputtedBytesMutex.unlock();

    mHandleTimer->start();
}
