/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QStandardPaths>
#include <QCoreApplication>

#include "SAKSettings.hh"
#include "SAKDataStructure.hh"

static const QString fileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini")
                   .arg(path, QCoreApplication::applicationName());
    return ret;
}

SAKSettings::SAKSettings(QObject *parent)
    : QSettings{::fileName(), QSettings::IniFormat, parent}
{
    qCInfo(mLoggingCategory) << "Settings file:" << fileName();
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
        if (ret != SAKDataStructure::HdpiPolicySystem) {
            ret = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        }
    }
#endif
    return ret;
}

void SAKSettings::setHdpiPolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if ((policy < 1) || (policy > 5)) {
        if (policy != SAKDataStructure::HdpiPolicySystem) {
            policy = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        }
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
    return value(mSettingsKey.appStyle, SAK_STYLE_DEFAULT).toString();
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

int SAKSettings::pageIndex()
{
    return value(mSettingsKey.pageIndex).toInt();
}

void SAKSettings::setPageIndex(int index)
{
    setValue(mSettingsKey.pageIndex, index);
    emit pageIndexChanged();
}

bool SAKSettings::isTextBesideIcon()
{
    return value(mSettingsKey.isTextBesideIcon).toBool();
}

void SAKSettings::setIsTextBesideIcon(bool is)
{
    setValue(mSettingsKey.isTextBesideIcon, is);
    emit isTextBesideIconChanged();
}

int SAKSettings::palette()
{
    int ret = value(mSettingsKey.palette).toInt();
    if ((ret != SAKDataStructure::PaletteSystem)
        && (ret != SAKDataStructure::PaletteLight)
        && (ret != SAKDataStructure::PaletteDark)
        && (ret != SAKDataStructure::PaletteCustom)) {
        ret = SAKDataStructure::PaletteSystem;
    }

    return ret;
}

void SAKSettings::setPalette(int p)
{
    setValue(mSettingsKey.palette, p);
    emit paletteChanged();
}

QString SAKSettings::customPalette()
{
    return value(mSettingsKey.customPalette).toString();
}

void SAKSettings::setCustomPalette(const QString &p)
{
    setValue(mSettingsKey.customPalette, p);
    emit customPaletteChanged();
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
