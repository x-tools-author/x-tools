/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "SAKSettings.h"

SAKSettings *SAKSettings::_sakSettings = NULL;
SAKSettings *sakSettings()
{
    return SAKSettings::_sakSettings;
}

SAKSettings::SAKSettings(const QString &fileName, Format format, QObject *parent)
    :QSettings(fileName, format, parent)
{
    _sakSettings = this;
}

SAKSettings::~SAKSettings()
{

}

QString SAKSettings::valueApplicationStyle()
{
    QSettings settings(SAKSETTINGFILE, QSettings::IniFormat);
    return settings.value(APPLICATIONSTYLE).toString();
}

void SAKSettings::setValueApplicationStyle(QString value)
{
    setValue(APPLICATIONSTYLE, value);
}
