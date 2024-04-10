/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSettings.h"

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

xToolsSettings::xToolsSettings(QObject* parent)
    : QSettings{::fileName(), QSettings::IniFormat, parent}
{
    qInfo() << "The path of settings file is:" << qPrintable(fileName());
}

xToolsSettings* xToolsSettings::instance()
{
    static xToolsSettings* settings = Q_NULLPTR;
    if (!settings) {
        settings = new xToolsSettings(qApp);
    }
    return settings;
}

void xToolsSettings::openSettingsFileDir()
{
    QDesktopServices::openUrl(settingsPath());
}

QString xToolsSettings::settingsPath()
{
    QString settingsFile(fileName());
    QString path = settingsFile.left(settingsFile.lastIndexOf("/"));
    return path;
}

int xToolsSettings::hdpiPolicy()
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

void xToolsSettings::setHdpiPolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    setValue(m_settingsKey.hdpiPolicy, policy);
    emit hdpiPolicyChanged();
#else
    Q_UNUSED(policy)
#endif
}

QString xToolsSettings::appStyle()
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

void xToolsSettings::setAppStyle(const QString& style)
{
    setValue(m_settingsKey.appStyle, style);
}

QString xToolsSettings::language()
{
    return value(m_settingsKey.language).toString();
}

void xToolsSettings::setLanguage(const QString& lan)
{
    setValue(m_settingsKey.language, lan);
}

bool xToolsSettings::clearSettings()
{
    return value(m_settingsKey.clearSettings).toBool();
}

void xToolsSettings::setClearSettings(bool clear)
{
    setValue(m_settingsKey.clearSettings, clear);
    emit clearSettingsChanged();
}

QString xToolsSettings::palette()
{
    return value(m_settingsKey.palette).toString();
}

void xToolsSettings::setPalette(const QString& fileName)
{
    setValue(m_settingsKey.palette, fileName);
    emit paletteChanged();
}

QVariant xToolsSettings::value(const QString& key, const QVariant& defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

void xToolsSettings::setValue(const QString& key, const QVariant& value)
{
    QSettings::setValue(key, value);
}
