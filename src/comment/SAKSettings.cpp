/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SAKSettings.h"

SAKSettings *SAKSettings::_sakSettings = nullptr;
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
    QString appStyle = settings.value(APPLICATIONSTYLE).toString();
    if (appStyle.isEmpty()){
        return "Fusion";
    }else {
        return appStyle;
    }
}

void SAKSettings::setValueApplicationStyle(QString value)
{
    setValue(APPLICATIONSTYLE, value);
}
