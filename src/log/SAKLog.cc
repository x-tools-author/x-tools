/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKLog.hh"
#include "SAKSettings.hh"

SAKLog::SAKLog(QObject *parent)
    : QThread{parent}
{

}

SAKLog::~SAKLog()
{
    qCDebug(mLoggingCategory) << __FUNCTION__;

    exit();
    wait();
}

QString SAKLog::fileName()
{
    return SAKSettings::instance()->value(mSettingsKey.fileNmae).toString();
}

void SAKLog::setFileName(const QString &name)
{
    SAKSettings::instance()->setValue(mSettingsKey.fileNmae, name);
}

SAKLog *SAKLog::instance()
{
    static SAKLog log;
    return &log;
}

void SAKLog::run()
{
    exec();
}
