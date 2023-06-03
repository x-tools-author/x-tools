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
    Q_PROPERTY(bool clearSettings READ clearSettings WRITE setClearSettings NOTIFY clearSettingsChanged)
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

    bool clearSettings();
    void setClearSettings(bool clear);

signals:
    void hdpiPolicyChanged();
    void uiTypeChanged();
    void clearSettingsChanged();

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
        const QString clearSettings{"clearSettings"};
    } mSettingsKey;
};

#endif // SAKSETTINGS_H
