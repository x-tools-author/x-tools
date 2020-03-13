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
#ifndef SAKSETTINGS_HH
#define SAKSETTINGS_HH

#include <QSettings>

class SAKSettings:public QSettings
{
    Q_OBJECT
    Q_PROPERTY(bool enableAutoCheckForUpdate READ enableAutoCheckForUpdate WRITE setEnableAutoCheckForUpdate NOTIFY enableAutoCheckForUpdateChanged)
    Q_PROPERTY(QString appStyle READ appStyle WRITE setAppStyle NOTIFY appStyleChanged)
    Q_PROPERTY(QString appStylesheet READ appStylesheet WRITE setAppStylesheet NOTIFY appStylesheetChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool isClassicalUi READ isClassicalUi WRITE setIsClassicalUi NOTIFY isClassicalUiChanged)
public:
    static SAKSettings* instance();
private:
    static SAKSettings* _instance;
    SAKSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr);
    ~SAKSettings();

    /// @brief 以下是软件自动更新使能设置的相关成员
public:    
    bool enableAutoCheckForUpdate();
    void setEnableAutoCheckForUpdate(bool enable);
private:
    QString enableAutoCheckForUpdateKey;
signals:
    void enableAutoCheckForUpdateChanged();

     /// @brief 以下软件风格设置相关成员
public:
    QString appStyle();
    void setAppStyle(QString style);
private:
    QString appStyleKey;
signals:
    void appStyleChanged();

     /// @brief 以下是软件样式设置相关成员
public:
    QString appStylesheet();
    void setAppStylesheet(QString stylesheet);
private:
    QString appStylesheetKey;
signals:
    void appStylesheetChanged();

    /// @brief 语言配置相关选项
public:
    QString language();
    void setLanguage(QString language);
private:
    QString languageKey;
signals:
    void languageChanged();

    /// @brief ui类型配置相关选项
public:
    bool isClassicalUi();
    void setIsClassicalUi(bool isClassicalUi);
private:
    QString isClassicalUiKey;
signals:
    void isClassicalUiChanged();
};

#endif
