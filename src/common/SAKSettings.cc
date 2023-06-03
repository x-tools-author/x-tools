/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QStandardPaths>
#include <QCoreApplication>

#include "SAKSettings.hh"

static const QString fileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini").arg(path, qApp->applicationName());
    return ret;
}

SAKSettings::SAKSettings(QObject *parent)
    : QSettings{::fileName(), QSettings::IniFormat, parent}
{

}

SAKSettings *SAKSettings::instance()
{
    static SAKSettings settings;
    return &settings;
}

int SAKSettings::hdpiPolicy()
{
    int ret = value(mSettingsKey.hdpiPolicy).toInt();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if ((ret < 1) || (ret > 5)) {
        ret = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    }
#endif
    return ret;
}

void SAKSettings::setHdpiPolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if ((policy < 1) || (policy > 5)) {
        policy = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    }
#endif

    setValue(mSettingsKey.hdpiPolicy, policy);
    emit hdpiPolicyChanged();
}

int SAKSettings::uiType()
{
    int ret = value(mSettingsKey.uiType).toInt();
    if ((ret < 0) || (ret > 1)) {
        ret = UiTypeWidget;
    }

    return ret;
}

void SAKSettings::setUiType(int type)
{
    if ((type < 0) || (type > 1)) {
        type = UiTypeWidget;
    }

    setValue(mSettingsKey.uiType, type);
    emit uiTypeChanged();
}

QString SAKSettings::appStyle()
{
    return value(mSettingsKey.appStyle).toString();
}

void SAKSettings::setAppStyle(const QString &style)
{
    setValue(mSettingsKey.appStyle, style);
}

QString SAKSettings::language()
{
    return value(mSettingsKey.language).toString();
}

void SAKSettings::setLanguage(const QString &lan)
{
    setValue(mSettingsKey.language, lan);
}

bool SAKSettings::clearSettings()
{
    return value(mSettingsKey.clearSettings).toBool();
}

void SAKSettings::setClearSettings(bool clear)
{
    setValue(mSettingsKey.clearSettings, clear);
    emit clearSettingsChanged();
}

QVariant SAKSettings::value(const QString &key,
                            const QVariant &defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

void SAKSettings::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, value);
}
