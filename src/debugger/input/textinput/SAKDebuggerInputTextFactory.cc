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

#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerInputSettings.hh"

SAKDebuggerTextInput::SAKDebuggerTextInput(SAKDebugger *debugger, QObject *parent)
    :QObject(parent)
    ,debugger(debugger)
    ,_addCRCFlag(false)
    ,_bigEndianCRCFlag(false)
{

}

SAKDebuggerTextInput::~SAKDebuggerTextInput()
{

}

void SAKDebuggerTextInput::writeBytes(QByteArray bytes)
{
    emit writeBytesRequest(bytes);
}

void SAKDebuggerTextInput::writeRawData(QString data)
{
    if (data.length()){
        emit writeBytesRequest(data.toLatin1());
    }else{
        emit writeBytesRequest(QByteArray("(empty)"));
    }
}

bool SAKDebuggerTextInput::paraAddCRCFlag()
{
    addCRCFlagMutex.lock();
    bool flag = _addCRCFlag;
    addCRCFlagMutex.unlock();

    return flag;
}

bool SAKDebuggerTextInput::paraBigEndianCRCFlag()
{
    bigEndianCRCFlagMutex.lock();
    bool flag = _bigEndianCRCFlag;
    bigEndianCRCFlagMutex.unlock();

    return flag;
}

bool SAKDebuggerTextInput::addCRCFlag()
{
    addCRCFlagMutex.lock();
    bool flag = _addCRCFlag;
    addCRCFlagMutex.unlock();

    return flag;
}

void SAKDebuggerTextInput::setAddCRCFlag(bool flag)
{
    addCRCFlagMutex.lock();
    _addCRCFlag = flag;
    addCRCFlagMutex.unlock();

    emit addCRCFlagChanged();
}

bool SAKDebuggerTextInput::bigEndianCRCFlag()
{
    bigEndianCRCFlagMutex.lock();
    bool flag = _bigEndianCRCFlag;
    bigEndianCRCFlagMutex.unlock();

    return flag;
}

void SAKDebuggerTextInput::setBigEndianCRCFlag(bool flag)
{
    bigEndianCRCFlagMutex.lock();
    _bigEndianCRCFlag = flag;
    bigEndianCRCFlagMutex.unlock();

    emit bigEndianCRCFlagChanged();
}
