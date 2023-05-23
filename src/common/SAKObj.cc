/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include "SAKObj.hh"

SAKObj::SAKObj(const char *lc)
    : mLoggingCategory(lc)
{

}

void SAKObj::printLog(QtMsgType type, const QString &log)
{
    if (type == QtDebugMsg) {
        qCDebug(mLoggingCategory) << log;
    } else if (type == QtInfoMsg) {
        qCInfo(mLoggingCategory) << log;
    } else if (type == QtWarningMsg) {
        qCWarning(mLoggingCategory) << log;
    } else {
        qCCritical(mLoggingCategory) << log;
    }
}
