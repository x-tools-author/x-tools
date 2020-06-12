/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSETTINGS_HH
#define SAKSETTINGS_HH

#include <QSettings>

/// @brief 配置文件读写类
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

    QString appStyle();
    void setAppStyle(QString style);

    QString appStylesheet();
    void setAppStylesheet(QString stylesheet);

    QString language();
    void setLanguage(QString language);
private:
    static SAKSettings* _instance;
private:
    QString enableAutoCheckForUpdateKey;
    QString appStyleKey;
    QString appStylesheetKey;
    QString languageKey;
};

#endif
