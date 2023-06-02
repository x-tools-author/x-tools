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
    if ((ret < 1) || (ret > 5)) {
        ret = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    }

    return ret;
}

void SAKSettings::setHdpiPolicy(int policy)
{
    if ((policy < 1) || (policy > 5)) {
        policy = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    }

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

QVariant SAKSettings::sakValue(const QString &key,
                             const QVariant &defaultValue) const
{
    return value(key, defaultValue);
}

void SAKSettings::sakSetValue(const QString &key, const QVariant &value)
{
    setValue(key, value);
}

void SAKSettings::sakSetArrayValues(const QString &groupName,
                                  const QString &array,
                                  const QString &key,
                                  const QVariant &varList)
{
    QVariantList cookedVarList = varList.toList();
    beginGroup(groupName);
    beginWriteArray(array);
    for (int i = 0; i < cookedVarList.count(); i++) {
        setArrayIndex(i);
        setValue(key, cookedVarList.at(i).toString());
    }
    endArray();
    endGroup();
}

QStringList SAKSettings::sakArrayValues(const QString &group,
                                      const QString &array,
                                      const QString &key)
{
    QStringList list;
    beginGroup(group);
    int size = beginReadArray(array);
    for (int i = 0; i < size ; i++) {
        setArrayIndex(i);
        list.append(value(key).toString());
    }
    endArray();
    endGroup();

    return list;
}

void SAKSettings::sakRemove(const QString &group, const QString &key)
{
    beginGroup(group);
    remove(key);
    endGroup();
}
