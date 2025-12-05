/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QSettings>
#include <QSplashScreen>
#include <QVariant>

#define xAPP (static_cast<xApp *>(QCoreApplication::instance()))

class xApp : public QApplication
{
    Q_OBJECT
public:
    explicit xApp(int &argc, char **argv);
    ~xApp() override;

    static void installLog(char *argv0);
    static void uninstallLog();
    static void setupHdpi();
    static QSettings *settings();
    static QString settingsPath();

    void setupAppStyle();
    void setupLanguage();
    void setupColorScheme();
    void execMs(int ms);

    QSplashScreen *splashScreen();
    Q_INVOKABLE void showSplashScreenMessage(const QString &msg);

signals:
    void languageChanged();
};