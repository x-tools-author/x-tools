/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <QMainWindow>
#include <QObject>
#include <QSplashScreen>

namespace xTools {
class xToolsPrivate;
class xTools : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(xTools)
private:
    explicit xTools(QObject *parent = Q_NULLPTR);
    xTools(const xTools &) = delete;
    xTools &operator=(const xTools &) = delete;

public:
    static xTools &signleton();

signals:
    void languageChanged();

public:
    // About splash screen
    Q_INVOKABLE bool enableSplashScreen();
    Q_INVOKABLE void setEnableSplashScreen(bool enable);
    Q_INVOKABLE void setSplashScreenMessage(const QString &msg);
    Q_INVOKABLE QSplashScreen *splashScreen();

    // About app info
    Q_INVOKABLE QString appFriendlyName();
    Q_INVOKABLE void setAppFriendlyName(const QString &name);
    Q_INVOKABLE QString appVersion();

    // About i18n
    Q_INVOKABLE QString defaultAppLanguage();
    Q_INVOKABLE QStringList supportedAppLanguages();
    Q_INVOKABLE QStringList supportedAppLanguagePrefixes();
    Q_INVOKABLE void setSupportedAppLanguagePrefixes(const QStringList &prefixes);
    Q_INVOKABLE void setupAppLanguageWithPrefix(const QString &language, const QString &prefix);
    Q_INVOKABLE void setupAppLanguage(const QString &language = QString());

    // About git info
    Q_INVOKABLE QString xToolsVersion();
    Q_INVOKABLE QString xToolsLastCommit();
    Q_INVOKABLE QString xToolsLastCommitTime();

    // About high dpi policy
    Q_INVOKABLE QVariantList supportedHdpiPolicies();
    Q_INVOKABLE QString hdpiPolicyName(int policy);
    Q_INVOKABLE bool isValidHdpiPolicy(int policy);

    // About formater
    Q_INVOKABLE QString stringToHexString(const QString &str);
    Q_INVOKABLE QString hexStringToString(const QString &str);
    Q_INVOKABLE QByteArray byteArray2Hex(const QByteArray &source, char separator = '\0');

    // About date time
    Q_INVOKABLE QString dateTimeString(const QString &format);
    Q_INVOKABLE QDateTime buildDateTime();
    Q_INVOKABLE QString buildDateTimeString(const QString &format);
    Q_INVOKABLE QString systemDateFormat();
    Q_INVOKABLE QString systemTimeFormat();

    // About system
    Q_INVOKABLE QString desktopPath();
    Q_INVOKABLE QString clipboardText();
    Q_INVOKABLE void setClipboardText(const QString &text);
    Q_INVOKABLE void openUrl(const QString &url);

    // About icon
    Q_INVOKABLE QIcon toThemeIcon(const QIcon &icon);
    Q_INVOKABLE QIcon cookedIconFile(const QString &iconFile, const QString &color);
    Q_INVOKABLE QIcon cookedIcon(const QIcon &icon, const QString &color);

    // Other functions
    Q_INVOKABLE QMainWindow *mainWindow();
    Q_INVOKABLE void moveToScreenCenter(QWidget *widget);
    Q_INVOKABLE void tryToReboot();
    Q_INVOKABLE void tryToClearSettings();
};

} // namespace xTools
