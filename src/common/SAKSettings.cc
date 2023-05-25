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

QVariant SAKSettings::sakValue(const QString &key,
                             const QVariant &defaultValue) const
{
    return value(key, defaultValue);
}

void SAKSettings::sdkSetValue(const QString &key, const QVariant &value)
{
    setValue(key, value);
}

void SAKSettings::sdkSetArrayValues(const QString &groupName,
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

QStringList SAKSettings::sdkArrayValues(const QString &group,
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

void SAKSettings::sdkRemove(const QString &group, const QString &key)
{
    beginGroup(group);
    remove(key);
    endGroup();
}
