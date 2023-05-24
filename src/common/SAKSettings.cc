/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QStandardPaths>
#include <QCoreApplication>

#include "EDSettings.hpp"

static const QString fileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini").arg(path, qApp->applicationName());
    return ret;
}

EDSettings::EDSettings(QObject *parent)
    : QSettings{::fileName(), QSettings::IniFormat, parent}
{

}

EDSettings *EDSettings::instance()
{
    static EDSettings settings;
    return &settings;
}

QVariant EDSettings::edValue(const QString &key,
                             const QVariant &defaultValue) const
{
    return value(key, defaultValue);
}

void EDSettings::edSetValue(const QString &key, const QVariant &value)
{
    setValue(key, value);
}

void EDSettings::edSetArrayValues(const QString &groupName,
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

QStringList EDSettings::edArrayValues(const QString &group,
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

void EDSettings::edRemove(const QString &group, const QString &key)
{
    beginGroup(group);
    remove(key);
    endGroup();
}
