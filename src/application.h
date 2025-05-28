/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QSettings>
#include <QSplashScreen>

#define xApp (static_cast<Application *>(QCoreApplication::instance()))

class ApplicationPrivate;
class Application : public QApplication
{
    Q_OBJECT
public:
    struct SettingsKey
    {
        const QString hdpi{"Application/hdpi"};
        const QString style{"Application/style"};
        const QString language{"Application/language"};
        const QString clearSettings{"Application/clearSettings"};
        const QString colorScheme{"colorScheme"};
    };

public:
    explicit Application(int &argc, char **argv);

    static void installLog(char *argv0);
    static void uninstallLog();
    static void setupHdpi();
    static QSettings *settings();
    static QString settingsPath();

    void setupAppStyle();
    void execMs(int ms);
    void setupLanguage();
    void setupColorScheme();

    QSplashScreen *splashScreen();
    Q_INVOKABLE void showSplashScreenMessage(const QString &msg);

    QString left2rightTips() const;
    QString right2leftTips() const;
};
