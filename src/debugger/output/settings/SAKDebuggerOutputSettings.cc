/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QMetaEnum>

#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerOutputStatistics.hh"

SAKDebuggerOutputSettings::SAKDebuggerOutputSettings(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
    ,_currentTextFormat ("Hex")
{
    _statistics = new SAKDebuggerOutputStatistics;
}

SAKDebuggerOutputSettings::~SAKDebuggerOutputSettings()
{
    delete _statistics;
}

SAKDebuggerOutputSettings::ParametersContext SAKDebuggerOutputSettings::parameters()
{
    ParametersContext paraCtx;
    parameterCtxMutex.lock();
    paraCtx.outputMs = parameterCtx.outputMs;
    paraCtx.outputRx = parameterCtx.outputRx;
    paraCtx.outputTx = parameterCtx.outputTx;
    paraCtx.outputDate = parameterCtx.outputDate;
    paraCtx.outputTime = parameterCtx.outputTime;
    paraCtx.outputWrap = parameterCtx.outputWrap;
    paraCtx.outputFormat = parameterCtx.outputFormat;
    parameterCtxMutex.unlock();

    return paraCtx;
}

SAKDebuggerOutputStatistics * SAKDebuggerOutputSettings::statisticsInstance()
{
    return _statistics;
}

void SAKDebuggerOutputSettings::clearOutput()
{
    emit clearOutputReauest();
}

void SAKDebuggerOutputSettings::saveOutputData()
{
    emit saveOutputDataRequest();
}

QStringList SAKDebuggerOutputSettings::textFormats()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<OutputTextFormat>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

SAKDebuggerOutputStatistics *SAKDebuggerOutputSettings::statistics()
{
    return _statistics;
}

QString SAKDebuggerOutputSettings::currentTextFormat()
{
    return _currentTextFormat;
}

void SAKDebuggerOutputSettings::setCurrentTextFormat(QString format)
{
    parameterCtxMutex.lock();
    QMetaEnum metaEnum = QMetaEnum::fromType<OutputTextFormat>();
    parameterCtx.outputFormat = metaEnum.keyToValue(format.toLatin1().data());
    parameterCtxMutex.unlock();

    _currentTextFormat = format;
    emit currentTextFormatChanged();
}

bool SAKDebuggerOutputSettings::outputDate()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputDate;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputDate(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputDate = enable;
    parameterCtxMutex.unlock();
    emit outputDateChanged();
}

bool SAKDebuggerOutputSettings::outputTime()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputTime;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputTime(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputTime = enable;
    parameterCtxMutex.unlock();
    emit outputTimeChanged();
}

bool SAKDebuggerOutputSettings::outputMs()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputMs;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputMs(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputMs = enable;
    parameterCtxMutex.unlock();
    emit outputMsChanged();
}

bool SAKDebuggerOutputSettings::outputRx()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputRx;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputRx(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputRx = enable;
    parameterCtxMutex.unlock();
    emit outputRxChanged();
}

bool SAKDebuggerOutputSettings::outputTx()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputTx;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputTx(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputTx = enable;
    parameterCtxMutex.unlock();
    emit outputTxChanged();
}

bool SAKDebuggerOutputSettings::outputWrap()
{
    parameterCtxMutex.lock();
    bool en = parameterCtx.outputWrap;
    parameterCtxMutex.unlock();
    return en;
}

void SAKDebuggerOutputSettings::setOutputWrap(bool enable)
{
    parameterCtxMutex.lock();
    parameterCtx.outputWrap = enable;
    parameterCtxMutex.unlock();
    emit outputWrapChanged();
}
