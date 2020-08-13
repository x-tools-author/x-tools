/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSETTINGS_HH
#define SAKSETTINGS_HH

#include <QSettings>

/// @brief 配置文件类
class SAKSettings:public QSettings
{
    Q_OBJECT
private:
    SAKSettings(const QString &fileName, QSettings::Format format, QObject *parent = Q_NULLPTR);
    ~SAKSettings();
public:
    /**
     * @brief instance 获取配置文件读写类实例指针
     * @return 配置文件读写类实例指针
     */
    static SAKSettings* instance();

    /// @brief 以下是软件自动更新使能设置的相关成员
    bool enableAutoCheckForUpdate();
    void setEnableAutoCheckForUpdate(bool enable);

    /// @brief 软件风格配置信息读写
    QString appStyle();
    void setAppStyle(QString style);

    /// @brief 软件样式配置信息读写
    QString appStylesheet();
    void setAppStylesheet(QString stylesheet);

    /// @brief 程序语言配置信息读写
    QString language();
    void setLanguage(QString language);

    /**
     * @brief fullPath: Get the full path of settings file
     * @return The full path of settings file
     */
    static const QString fullPath();
private:
    static SAKSettings* instancePtr;
private:
    QString enableAutoCheckForUpdateKey;
    QString appStyleKey;
    QString appStylesheetKey;
    QString languageKey;
};

#endif
