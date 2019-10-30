/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include "SAKGlobal.hh"
#include "SAKSettings.hh"

#include <QApplication>

SAKSettings* SAKSettings::_instance = nullptr;
SAKSettings* SAKSettings::instance()
{
    if (!_instance){
        const QString fileName = QString("%1/%2.ini").arg(SAKGlobal::dataPath()).arg(qApp->applicationName());
        new SAKSettings(fileName, QSettings::IniFormat, qApp);
    }

    return _instance;
}

SAKSettings::SAKSettings(const QString &fileName, Format format, QObject *parent)
    :QSettings(fileName, format, parent)
{
    _instance = this;
}

SAKSettings::~SAKSettings()
{
    _instance = nullptr;
}
