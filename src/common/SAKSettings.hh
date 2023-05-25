/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef SAKSETTINGS_H
#define SAKSETTINGS_H

#include <QSettings>

class SAKSettings : public QSettings
{
    Q_OBJECT
private:
    explicit SAKSettings(QObject *parent = nullptr);

public:
    static SAKSettings *instance();

    Q_INVOKABLE QVariant sakValue(
            const QString &key,
            const QVariant &defaultValue = QVariant()) const;
    Q_INVOKABLE void sdkSetValue(const QString &key, const QVariant &value);
    Q_INVOKABLE void sdkSetArrayValues(const QString &groupName,
                                       const QString &array,
                                       const QString &key,
                                       const QVariant &varList);
    Q_INVOKABLE QStringList sdkArrayValues(const QString &group,
                                           const QString &array,
                                           const QString &key);
    Q_INVOKABLE void sdkRemove(const QString &group, const QString &key);
};

#endif // SAKSETTINGS_H
