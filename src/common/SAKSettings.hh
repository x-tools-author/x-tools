/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef SAKSETTINGS_H
#define SAKSETTINGS_H

#include <Qt>
#include <QSettings>

class SAKSettings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int hdpiPolicy READ hdpiPolicy WRITE setHdpiPolicy NOTIFY hdpiPolicyChanged)
    Q_PROPERTY(int uiType READ uiType WRITE setUiType NOTIFY uiTypeChanged)
public:
    enum UiType {
        UiTypeWidget,
        UiTypeQml,
    };
    Q_ENUM(UiType)

private:
    explicit SAKSettings(QObject *parent = nullptr);

public:
    static SAKSettings *instance();

    int hdpiPolicy();
    void setHdpiPolicy(int policy);

    int uiType();
    void setUiType(int type);

signals:
    void hdpiPolicyChanged();
    void uiTypeChanged();

public:
    Q_INVOKABLE QVariant sakValue(const QString &key,
                                  const QVariant &defaultValue
                                  = QVariant()) const;
    Q_INVOKABLE void sakSetValue(const QString &key, const QVariant &value);
    Q_INVOKABLE void sakSetArrayValues(const QString &groupName,
                                       const QString &array,
                                       const QString &key,
                                       const QVariant &varList);
    Q_INVOKABLE QStringList sakArrayValues(const QString &group,
                                           const QString &array,
                                           const QString &key);
    Q_INVOKABLE void sakRemove(const QString &group, const QString &key);

private:
    struct {
        const QString hdpiPolicy{"hdpiPolicy"};
        const QString uiType{"uiType"};
    } mSettingsKey;
};

#endif // SAKSETTINGS_H
