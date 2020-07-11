/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
