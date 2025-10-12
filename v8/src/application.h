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
#include <QJsonArray>
#include <QSettings>
#include <QSplashScreen>
#include <QStyleHints>
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

#if 0
    enum class Theme {
        System = static_cast<int>(Qt::ColorScheme::Unknown),
        Light = static_cast<int>(Qt::ColorScheme::Unknown),
        Dark = static_cast<int>(Qt::ColorScheme::Unknown)
    };
    Q_ENUM(Theme)
#endif

#if 0
    enum class HdpiPolicy {
        Round = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        Ceil = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        Floor = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
        RoundPreferFloor = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
        PassThrough = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough)
    };
    Q_ENUM(HdpiPolicy)
#endif

signals:
    void languageChanged();

public:
    explicit Application(int &argc, char **argv);
    ~Application() override;

    Q_INVOKABLE QString gitCommit();
    Q_INVOKABLE QString buildDateTime(const QString &format = QString());
    Q_INVOKABLE QJsonArray supportedLanguages();

    QSettings *settings();
    QString settingsPath();
    Q_INVOKABLE QVariant settingsValue(const QString &key,
                                       const QVariant &defaultValue = QVariant());
    Q_INVOKABLE void setSettingsValue(const QString &key, const QVariant &value);
    Q_INVOKABLE QString appLanguageFlag();

    QSplashScreen *splashScreen();
    void showSplashScreenMessage(const QString &msg);

    void execMs(int ms);
    void tryToReboot();

    void setupHdpi();
    void setupLanguage();
    void setupTheme();
};
