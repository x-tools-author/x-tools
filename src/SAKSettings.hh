/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKSETTINGS_HH
#define SAKSETTINGS_HH

#include <QSettings>

class SAKSettings:public QSettings
{
    Q_OBJECT
public:
    static SAKSettings* instance();
private:
    static SAKSettings* _instance;
    SAKSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr);
    ~SAKSettings();

public:
    QString settingStringEnableAutoUpdate;
    bool enableAutoCheckForUpdate();
    void setEnableAutoCheckForUpdate(bool enable);
};

#endif
