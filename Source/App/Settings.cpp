/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Settings.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QStyle>
#include <QUrl>

namespace xTools {

struct SettingKeys
{
    const QString hdpiPolicy{"hdpiPolicy"};
    const QString uiType{"uiType"};
    const QString appStyle{"appStyle"};
    const QString language{"language"};
    const QString clearSettings{"clearSettings"};
    const QString pageIndex{"pageIndex"};
    const QString isTextBesideIcon{"isTextBesideIcon"};
    const QString palette{"palette"};
    const QString customPalette{"customPalette"};
    const QString colorScheme{"colorScheme"};
} g_keys;

static const QString settingfileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini").arg(path, QCoreApplication::applicationName());
    return ret;
}

Settings::Settings(QObject* parent)
    : QSettings{settingfileName(), QSettings::IniFormat, parent}
{
    qInfo() << "The path of settings file is:" << qPrintable(fileName());
}

Settings* xTools::Settings::instance()
{
    static Settings* settings = Q_NULLPTR;
    if (!settings) {
        settings = new Settings(qApp);
    }
    return settings;
}

void Settings::openSettingsFileDir()
{
    QDesktopServices::openUrl(settingsPath());
}

QString Settings::settingsPath()
{
    QString settingsFile(fileName());
    QString path = settingsFile.left(settingsFile.lastIndexOf("/"));
    return path;
}

int Settings::hdpiPolicy()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto var = value(g_keys.hdpiPolicy);
    if (var.isValid()) {
        return value(g_keys.hdpiPolicy).toInt();
    }

    return int(QGuiApplication::highDpiScaleFactorRoundingPolicy());
#else
    return 0;
#endif
}

void Settings::setHdpiPolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    setValue(g_keys.hdpiPolicy, policy);
    emit hdpiPolicyChanged();
#else
    Q_UNUSED(policy)
#endif
}

QString Settings::appStyle()
{
    auto var = value(g_keys.appStyle);
    if (var.isValid()) {
        return value(g_keys.appStyle).toString();
    }

    if (QApplication::instance()) {
        return QApplication::style()->objectName();
    }

    return QString("");
}

void Settings::setAppStyle(const QString& style)
{
    setValue(g_keys.appStyle, style);
}

QString Settings::language()
{
    return value(g_keys.language).toString();
}

void Settings::setLanguage(const QString& lan)
{
    setValue(g_keys.language, lan);
}

bool Settings::clearSettings()
{
    return value(g_keys.clearSettings).toBool();
}

void Settings::setClearSettings(bool clear)
{
    setValue(g_keys.clearSettings, clear);
    emit clearSettingsChanged();
}

QString Settings::palette()
{
    return value(g_keys.palette).toString();
}

void Settings::setPalette(const QString& fileName)
{
    setValue(g_keys.palette, fileName);
    emit paletteChanged();
}

int Settings::colorScheme()
{
    return value(g_keys.colorScheme).toInt();
}

void Settings::setColorScheme(const int colorScheme)
{
    setValue(g_keys.colorScheme, colorScheme);
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value)
{
    QSettings::setValue(key, value);
}

void Settings::setJsonObjectStringValue(const QString& key, const QString& value)
{
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if (doc.isNull()) {
        return;
    }

    setValue(key, doc.toVariant());
}

} // namespace xTools
