/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QStandardPaths>
#include <QStyle>
#include <QUrl>

static const QString fileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini").arg(path, QCoreApplication::applicationName());
    return ret;
}

Settings::Settings(QObject* parent)
    : QSettings{::fileName(), QSettings::IniFormat, parent}
{
    qInfo() << "The path of settings file is:" << qPrintable(fileName());
}

Settings* Settings::instance()
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
    auto var = value(m_settingsKey.hdpiPolicy);
    if (var.isValid()) {
        return value(m_settingsKey.hdpiPolicy).toInt();
    }

    return int(QGuiApplication::highDpiScaleFactorRoundingPolicy());
#else
    return 0;
#endif
}

void Settings::setHdpiPolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    setValue(m_settingsKey.hdpiPolicy, policy);
    emit hdpiPolicyChanged();
#else
    Q_UNUSED(policy)
#endif
}

QString Settings::appStyle()
{
    auto var = value(m_settingsKey.appStyle);
    if (var.isValid()) {
        return value(m_settingsKey.appStyle).toString();
    }

    if (QApplication::instance()) {
        return QApplication::style()->objectName();
    }

    return QString("");
}

void Settings::setAppStyle(const QString& style)
{
    setValue(m_settingsKey.appStyle, style);
}

QString Settings::language()
{
    return value(m_settingsKey.language).toString();
}

void Settings::setLanguage(const QString& lan)
{
    setValue(m_settingsKey.language, lan);
}

bool Settings::clearSettings()
{
    return value(m_settingsKey.clearSettings).toBool();
}

void Settings::setClearSettings(bool clear)
{
    setValue(m_settingsKey.clearSettings, clear);
    emit clearSettingsChanged();
}

QString Settings::palette()
{
    return value(m_settingsKey.palette).toString();
}

void Settings::setPalette(const QString& fileName)
{
    setValue(m_settingsKey.palette, fileName);
    emit paletteChanged();
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value)
{
    QSettings::setValue(key, value);
}
