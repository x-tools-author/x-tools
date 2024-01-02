/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakanalyzertool.h"
#include "sakinterface.h"

SAKAnalyzerTool::SAKAnalyzerTool(QObject *parent)
    : SAKBaseTool{parent}
{
    m_enable = false;
}

void SAKAnalyzerTool::setFixed(bool fixed)
{
    m_parametersMutex.lock();
    m_parameters.fixed = fixed;
    m_parametersMutex.unlock();
}

void SAKAnalyzerTool::setFrameBytes(int bytes)
{
    m_parametersMutex.lock();
    m_parameters.frameBytes = bytes;
    m_parametersMutex.unlock();
}

void SAKAnalyzerTool::setSeparationMark(const QByteArray &mark)
{
    m_parametersMutex.lock();
    m_parameters.separationMark = mark;
    m_parametersMutex.unlock();
}

void SAKAnalyzerTool::setMaxTempBytes(int maxBytes)
{
    m_parametersMutex.lock();
    m_parameters.maxTempBytes = maxBytes;
    m_parametersMutex.unlock();
}

void SAKAnalyzerTool::inputBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        qInfo() << "Empty input bytes, the operation will be ignored!";
        return;
    }

    if (isEnable()) {
        m_inputtedBytesMutex.lock();
        m_inputtedBytes.append(bytes);
        m_inputtedBytesMutex.unlock();
    } else {
        emit outputBytes(bytes);
    }
}

void SAKAnalyzerTool::run()
{
    QTimer *handleTimer = new QTimer();
    handleTimer->setInterval(5);
    handleTimer->setSingleShot(true);
    connect(handleTimer, &QTimer::timeout, handleTimer, [=]() {
        if (m_enable) {
            m_inputtedBytesMutex.lock();
            analyze();
            m_inputtedBytesMutex.unlock();
        }
        handleTimer->start();
    });
    handleTimer->start();

    exec();

    handleTimer->stop();
    handleTimer->deleteLater();
    handleTimer->stop();
    handleTimer->deleteLater();

    m_inputtedBytesMutex.lock();
    m_inputtedBytes.clear();
    m_inputtedBytesMutex.unlock();
}

void SAKAnalyzerTool::analyze()
{
    m_parametersMutex.lock();
    auto ctx = m_parameters;
    m_parametersMutex.unlock();

    if (ctx.fixed) {
        analyzeFixed();
    } else {
        analyzeSeparationMark();
    }

    if (m_inputtedBytes.length() > ctx.maxTempBytes) {
        QByteArray ba = SAKInterface::arrayToHex(m_inputtedBytes, ' ');
        qInfo() << "clear bytes: " + QString::fromLatin1(ba);
        emit outputBytes(m_inputtedBytes);
        m_inputtedBytes.clear();
    }
}

void SAKAnalyzerTool::analyzeFixed()
{
    m_parametersMutex.lock();
    auto ctx = m_parameters;
    m_parametersMutex.unlock();

    while (m_inputtedBytes.length() >= ctx.frameBytes) {
        QByteArray frame(m_inputtedBytes.data(), ctx.frameBytes);
        m_inputtedBytes.remove(0, ctx.frameBytes);

        QByteArray ba = SAKInterface::arrayToHex(frame, ' ');
        QString hex = QString::fromLatin1(ba);
        qInfo() << QString("Analyzer->%1").arg(hex);
        emit outputBytes(frame);
    }
}

void SAKAnalyzerTool::analyzeSeparationMark()
{
    if (m_inputtedBytes.isEmpty()) {
        return;
    }

    m_parametersMutex.lock();
    auto ctx = m_parameters;
    m_parametersMutex.unlock();
    if (ctx.separationMark.isEmpty()) {
        QByteArray ba = SAKInterface::arrayToHex(m_inputtedBytes, ' ');
        QString hex = QString::fromLatin1(ba);
        QString msg = QString("Analyzer->%1").arg(hex);
        qInfo() << msg;
        emit outputBytes(m_inputtedBytes);
        m_inputtedBytes.clear();
        return;
    }

    auto ret = m_inputtedBytes.indexOf(ctx.separationMark);
    if (ret == -1) {
        return;
    }

    int len = ret + ctx.separationMark.length();
    QByteArray frame(m_inputtedBytes.constData(), len);
    m_inputtedBytes.remove(0, len);

    QByteArray ba = SAKInterface::arrayToHex(frame, ' ');
    QString hex = QString::fromLatin1(ba);
    QString msg = QString("Analyzer->%1").arg(hex);
    qInfo() << msg;
    emit outputBytes(frame);
}
