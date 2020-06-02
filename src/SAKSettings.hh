/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
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
    SAKSettings(const QString &fileName, QSettings::Format format, QObject *parent = Q_NULLPTR);
    ~SAKSettings();

    /// @brief 以下是软件自动更新使能设置的相关成员
public:    
    bool enableAutoCheckForUpdate();
    void setEnableAutoCheckForUpdate(bool enable);

    QString appStyle();
    void setAppStyle(QString style);

    QString appStylesheet();
    void setAppStylesheet(QString stylesheet);

    QString language();
    void setLanguage(QString language);
private:
    QString enableAutoCheckForUpdateKey;
    QString appStyleKey;
    QString appStylesheetKey;
    QString languageKey;
};

#endif
