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
        const QString hdpi{"Application/highDpiPolicy"};
        const QString language{"Application/language"};
        const QString theme{"Application/theme"};
    };

    enum class HdpiPolicy {
        Round = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        Ceil = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        Floor = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
        RoundPreferFloor = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
        PassThrough = static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough)
    };
    Q_ENUM(HdpiPolicy)

signals:
    void languageChanged();

public:
    explicit Application(int &argc, char **argv);
    ~Application() override;

    Q_INVOKABLE QString gitCommit();
    Q_INVOKABLE QString buildDateTime(const QString &format = QString());
    Q_INVOKABLE QJsonArray supportedLanguages();
    Q_INVOKABLE QString appLanguageFlag();

    QSettings *settings();
    QString settingsPath();
    Q_INVOKABLE QVariant settingsValue(const QString &key, const QVariant &dv = QVariant());
    Q_INVOKABLE void setSettingsValue(const QString &key, const QVariant &value);

    QSplashScreen *splashScreen();
    void showSplashScreenMessage(const QString &msg);
    Q_INVOKABLE void updateWindowStyle(QWindow *window, const QColor &color);

    void execMs(int ms);
    void tryToReboot();

    void setupHdpi();
    Q_INVOKABLE void setupLanguage();
    void setupLanguage(const QString &qmFile);
    void setupTheme();

private:
    QList<QTranslator *> m_translators;
};
