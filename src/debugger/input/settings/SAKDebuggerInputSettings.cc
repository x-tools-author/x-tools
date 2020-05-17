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
#include <QMetaEnum>

#include "SAKCRCInterface.hh"
#include "SAKDebuggerInputSettings.hh"

#define MINI_CYCLIC_TIME 50

SAKDebuggerInputSettings::SAKDebuggerInputSettings(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
    ,_cyclicTime (MINI_CYCLIC_TIME)
{
    connect(&cyclicTimer, &QTimer::timeout, this, [&](){
        emit writeBytesRequest();
    });
}

SAKDebuggerInputSettings::~SAKDebuggerInputSettings()
{

}

QStringList SAKDebuggerInputSettings::avalidCRCParameterModel()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

void SAKDebuggerInputSettings::startTimer(bool startRequest)
{
    if (startRequest){
        cyclicTimer.start(_cyclicTime >= MINI_CYCLIC_TIME ? _cyclicTime : MINI_CYCLIC_TIME);
    }else{
        cyclicTimer.stop();
    }
}

void SAKDebuggerInputSettings::writeBytes()
{
    emit writeBytesRequest();
}

int SAKDebuggerInputSettings::paraCurrentCRCModel()
{
    currentCRCModelMutex.lock();
    int model = _currentCRCModel;
    currentCRCModelMutex.unlock();

    return model;
}

int SAKDebuggerInputSettings::paraCurrentInputFromat()
{
    currentInputFormatMutex.lock();
    int ret = _currentInputFormat;
    currentInputFormatMutex.unlock();

    return ret;
}

QStringList SAKDebuggerInputSettings::crcParameterModel()
{
    return avalidCRCParameterModel();
}

QStringList SAKDebuggerInputSettings::textFormats()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<InputTextFormat>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

QString SAKDebuggerInputSettings::cyclicTime()
{
    return QString::number(_cyclicTime);
}

void SAKDebuggerInputSettings::setCyclicTime(QString ct)
{
    quint32 ret = ct.toUInt();
    _cyclicTime = ret >= MINI_CYCLIC_TIME ? ret : MINI_CYCLIC_TIME;
    emit cyclicTimeChanged();
}

QString SAKDebuggerInputSettings::currentCRCModel()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    currentCRCModelMutex.lock();
    QString ret = QString(metaEnum.valueToKey(_currentCRCModel));
    currentCRCModelMutex.unlock();
    return ret;
}

void SAKDebuggerInputSettings::setCurrentCRCModel(QString model)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    int ret = metaEnum.keyToValue(model.toLatin1().data());
    currentCRCModelMutex.lock();
    _currentCRCModel = ret;
    currentCRCModelMutex.unlock();
    emit currentCRCModelChanged();
}

QString SAKDebuggerInputSettings::currentInputFromat()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<InputTextFormat>();
    currentInputFormatMutex.lock();
    QString ret = QString(metaEnum.valueToKey(_currentInputFormat));
    currentInputFormatMutex.unlock();
    return ret;
}

void SAKDebuggerInputSettings::setCurrentInputFromat(QString model)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<InputTextFormat>();
    int ret = metaEnum.keyToValue(model.toLatin1().data());
    currentInputFormatMutex.lock();
    _currentInputFormat = ret;
    currentInputFormatMutex.unlock();
    emit currentInputFromatChanged();
}
