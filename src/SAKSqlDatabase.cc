/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QCoreApplication>

#include "SAKGlobal.hh"
#include "SAKSettings.hh"

SAKSettings* SAKSettings::instancePtr = Q_NULLPTR;
SAKSettings* SAKSettings::instance()
{
    if (!instancePtr){
        const QString fileName = QString("%1/%2.ini").arg(SAKGlobal::dataPath()).arg(qApp->applicationName());
        new SAKSettings(fileName, QSettings::IniFormat, qApp);
    }
    Q_ASSERT_X(instancePtr, __FUNCTION__, "Initialzing failed!");

    return instancePtr;
}

SAKSettings::SAKSettings(const QString &fileName, Format format, QObject *parent)
    :QSettings(fileName, format, parent)
{
    instancePtr = this;

    enableAutoCheckForUpdateKey = QString("Universal/enableAutoCheckForUpdate");
    appStyleKey = QString("Universal/appStyle");
    appStylesheetKey = QString("Universal/appStylesheet");
    languageKey = QString("Universal/language");
}

SAKSettings::~SAKSettings()
{
    instancePtr = Q_NULLPTR;
}

bool SAKSettings::enableAutoCheckForUpdate()
{
    bool enable = value(enableAutoCheckForUpdateKey).toBool();
    return enable;
}

void SAKSettings::setEnableAutoCheckForUpdate(bool enable)
{    
    setValue(enableAutoCheckForUpdateKey, enable);
}

QString SAKSettings::appStyle()
{
    return value(appStyleKey).toString();
}

void SAKSettings::setAppStyle(QString style)
{
    setValue(appStyleKey, style);
}

QString SAKSettings::appStylesheet()
{
    return value(appStylesheetKey).toString();
}

void SAKSettings::setAppStylesheet(QString stylesheet)
{
    setValue(appStylesheetKey, stylesheet);
}

QString SAKSettings::language()
{
    return value(languageKey).toString();
}

void SAKSettings::setLanguage(QString language)
{
    setValue(languageKey, language);
}
