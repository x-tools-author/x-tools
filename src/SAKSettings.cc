/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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

    enableAutoCheckForUpdateKey = QString("Universal/enableAutoCheckForUpdate");
    appStyleKey = QString("Universal/appStyle");
    appStylesheetKey = QString("Universal/appStylesheet");
    languageKey = QString("Universal/language");
    isClassicalUiKey = QString("Universal/isClassicalUi");
}

SAKSettings::~SAKSettings()
{
    _instance = nullptr;
}

bool SAKSettings::enableAutoCheckForUpdate()
{
    bool enable = value(enableAutoCheckForUpdateKey).toBool();
    return enable;
}

void SAKSettings::setEnableAutoCheckForUpdate(bool enable)
{    
    setValue(enableAutoCheckForUpdateKey, enable);
    emit enableAutoCheckForUpdateChanged();
}

QString SAKSettings::appStyle()
{
    return value(appStyleKey).toString();
}

void SAKSettings::setAppStyle(QString style)
{
    setValue(appStyleKey, style);
    emit appStyleChanged();
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
    emit languageChanged();
}

bool SAKSettings::isClassicalUi()
{
    return value(isClassicalUiKey).toBool();
}

void SAKSettings::setIsClassicalUi(bool isClassicalUi)
{
    setValue(isClassicalUiKey, isClassicalUi);
    emit isClassicalUiChanged();
}
