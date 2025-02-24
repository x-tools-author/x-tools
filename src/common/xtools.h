/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QDateTime>
#include <QMainWindow>
#include <QObject>
#include <QSettings>
#include <QSplashScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>

#include "MainWindow.h"

#ifdef X_TOOLS_ENABLE_QSS
#include "qss/QssMgr.h"
#endif

#define g_xTools xTools::xTools::singleton()

static void (*gOutputLog2Ui)(QtMsgType, const QMessageLogContext &, const QString &);

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
    static xTools &singleton();
    static void doSomethingBeforeAppCreated(char *argv[],
                                            const QString &appName,
                                            const QString &appVersion = QString("0.0.0"),
                                            bool forStore = false);
    static void doSomethingAfterAppExited();
    static void googleLogInitializing(char *argv0);
    static void googleLogShutdown();
    static void googleLogToQtLog(QtMsgType type,
                                 const QMessageLogContext &context,
                                 const QString &msg);
    // About git info
    Q_INVOKABLE QString static xToolsVersion();
    Q_INVOKABLE QString static xToolsLastCommit();
    Q_INVOKABLE QString static xToolsLastCommitTime();

signals:
    void languageChanged();

public:
    // About splash screen
    Q_INVOKABLE bool splashScreenIsEnable();
    Q_INVOKABLE void splashScreenSetIsEnable(bool enable);
    Q_INVOKABLE void splashScreenSetMessage(const QString &msg);
    Q_INVOKABLE void splashScreenShow();
    Q_INVOKABLE QSplashScreen *splashScreenGet();

    // About app info
    Q_INVOKABLE QString appFriendlyName();
    Q_INVOKABLE void appSetFriendlyName(const QString &name);
    Q_INVOKABLE void appInitializeHdpi(const QString &appName, bool forStore);
    Q_INVOKABLE QString appVersion();

    // About i18n
    Q_INVOKABLE QString languageDefaultLanguage();
    Q_INVOKABLE QStringList languageSupportedLanguages();
    Q_INVOKABLE QStringList languageSupportedPrefixes();
    Q_INVOKABLE void languageSetSupportedPrefixes(const QStringList &prefixes);
    Q_INVOKABLE void languageSetupAppLanguageWithPrefix(const QString &language,
                                                        const QString &prefix);
    Q_INVOKABLE void languageSetupAppLanguage(const QString &language = QString());

    // About high dpi policy
    Q_INVOKABLE QVariantList hdpiSupportedPolicies();
    Q_INVOKABLE QString hdpiPolicyName(int policy);
    Q_INVOKABLE bool hdpiIsValidPolicy(int policy);

    // About formater
    Q_INVOKABLE QString formatStringToHexString(const QString &str);
    Q_INVOKABLE QString formatHexStringToString(const QString &str);
    Q_INVOKABLE QByteArray formatByteArray2Hex(const QByteArray &source, char separator = '\0');

    // About date time
    Q_INVOKABLE QString dtDateTimeString(const QString &format);
    Q_INVOKABLE QDateTime dtBuildDateTime();
    Q_INVOKABLE QString dtBuildDateTimeString(const QString &format);
    Q_INVOKABLE QString dtSystemDateFormat();
    Q_INVOKABLE QString dtSystemTimeFormat();

    // About system
    Q_INVOKABLE QString sysDesktopPath();
    Q_INVOKABLE QString sysClipboardText();
    Q_INVOKABLE void sysSetClipboardText(const QString &text);
    Q_INVOKABLE void sysOpenUrl(const QString &url);

    // About icon
    Q_INVOKABLE QIcon iconToThemeIcon(const QIcon &icon);
    Q_INVOKABLE QIcon iconCookedIconFile(const QString &iconFile, const QString &color);
    Q_INVOKABLE QIcon iconCookedIcon(const QIcon &icon, const QString &color);

    // About settings
    Q_INVOKABLE void settingsOpenSettingsFileDir();
    Q_INVOKABLE QString settingsPath();
    Q_INVOKABLE int settingsHdpiPolicy();
    Q_INVOKABLE void settingsSetHdpiPolicy(int policy);
    Q_INVOKABLE QString settingsAppStyle();
    Q_INVOKABLE void settingsSetAppStyle(const QString &style);
    Q_INVOKABLE QString settingsLanguage();
    Q_INVOKABLE void settingsSetLanguage(const QString &lan);
    Q_INVOKABLE bool settingsClearSettings();
    Q_INVOKABLE void settingsSetClearSettings(bool clear);
    Q_INVOKABLE int settingsColorScheme();
    Q_INVOKABLE void settingsSetColorScheme(const int colorScheme);
    Q_INVOKABLE QVariant settingsValue(const QString &key, const QVariant &value = QVariant()) const;
    Q_INVOKABLE void settingsSetValue(const QString &key, const QVariant &value);
    Q_INVOKABLE void settingsSetJsonObjectStringValue(const QString &key, const QString &value);
    Q_INVOKABLE QSettings *settings();

    // Other functions
    Q_INVOKABLE QMainWindow *mainWindow();
    Q_INVOKABLE void moveToScreenCenter(QWidget *widget);
    Q_INVOKABLE bool tryToReboot();
    Q_INVOKABLE void tryToClearSettings();
};

}