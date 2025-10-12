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

#define xApp (static_cast<Application *>(QCoreApplication::instance()))

class Application : public QApplication
{
    Q_OBJECT
public:
    struct SettingKeys
    {
        const QString hdpi{"Application/hdpi"};
        const QString language{"Application/language"};
        const QString theme{"Application/theme"};
    };

signals:
    void languageChanged();

public:
    explicit Application(int &argc, char **argv);
    ~Application() override;

    Q_INVOKABLE QString gitCommit();
    Q_INVOKABLE QString buildDateTime(const QString &format = QString());

    QSettings *settings();
    QString settingsPath();
    QVariant value(QAnyStringView key, const QVariant &defaultValue = QVariant());
    void setValue(QAnyStringView key, const QVariant &value);
    QString appLanguageFlag();

    QSplashScreen *splashScreen();
    void showSplashScreenMessage(const QString &msg);

    void execMs(int ms);
    void tryToReboot();

    void setupHdpi();
    void setupLanguage();
    void setupTheme();
};
