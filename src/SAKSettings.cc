/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
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

    settingStringEnableAutoUpdate = QString("Universal/enableAutoCheckForUpdate");
}

SAKSettings::~SAKSettings()
{
    _instance = nullptr;
}

bool SAKSettings::enableAutoCheckForUpdate()
{
    bool enable = value(settingStringEnableAutoUpdate).toBool();
    return enable;
}

void SAKSettings::setEnableAutoCheckForUpdate(bool enable)
{
    setValue(settingStringEnableAutoUpdate, enable);
}
