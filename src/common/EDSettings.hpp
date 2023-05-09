/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDSETTINGS_HPP
#define EDSETTINGS_HPP

#include <QSettings>

class EDSettings : public QSettings
{
    Q_OBJECT
private:
    explicit EDSettings(QObject *parent = nullptr);

public:
    static EDSettings *instance();
    Q_INVOKABLE QVariant edValue(
            const QString &key,
            const QVariant &defaultValue = QVariant()) const;
    Q_INVOKABLE void edSetValue(const QString &key, const QVariant &value);
    Q_INVOKABLE void edSetArrayValues(const QString &groupName,
                                      const QString &array,
                                      const QString &key,
                                      const QVariant &varList);
    Q_INVOKABLE QStringList edArrayValues(const QString &group,
                                          const QString &array,
                                          const QString &key);
    Q_INVOKABLE void edRemove(const QString &group, const QString &key);
};

#endif // EDSETTINGS_HPP
