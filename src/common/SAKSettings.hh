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

    QString appStyle();
    void setAppStyle(const QString &style);

    QString language();
    void setLanguage(const QString &lan);

signals:
    void hdpiPolicyChanged();
    void uiTypeChanged();

public:
    Q_INVOKABLE QVariant value(const QString &key,
                               const QVariant &defaultValue
                               = QVariant()) const;
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);

private:
    struct {
        const QString hdpiPolicy{"hdpiPolicy"};
        const QString uiType{"uiType"};
        const QString appStyle{"appStyle"};
        const QString language{"language"};
    } mSettingsKey;
};

#endif // SAKSETTINGS_H
