/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSETTINGS_H
#define SAKSETTINGS_H

#include <Qt>
#include <QSettings>
#include <QLoggingCategory>

class SAKSettings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int hdpiPolicy READ hdpiPolicy WRITE setHdpiPolicy NOTIFY hdpiPolicyChanged)
    Q_PROPERTY(int uiType READ uiType WRITE setUiType NOTIFY uiTypeChanged)
    Q_PROPERTY(bool clearSettings READ clearSettings WRITE setClearSettings NOTIFY clearSettingsChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(bool isTextBesideIcon READ isTextBesideIcon WRITE setIsTextBesideIcon NOTIFY isTextBesideIconChanged)
    Q_PROPERTY(int palette READ palette WRITE setPalette NOTIFY paletteChanged)
    Q_PROPERTY(QString customPalette READ customPalette WRITE setCustomPalette NOTIFY customPaletteChanged)
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

    int pageIndex();
    void setPageIndex(int index);

    bool isTextBesideIcon();
    void setIsTextBesideIcon(bool is);

    int palette();
    void setPalette(int p);

    QString customPalette();
    void setCustomPalette(const QString &p);

signals:
    void hdpiPolicyChanged();
    void uiTypeChanged();
    void clearSettingsChanged();
    void pageIndexChanged();
    void isTextBesideIconChanged();
    void paletteChanged();
    void customPaletteChanged();

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
        const QString pageIndex{"pageIndex"};
        const QString isTextBesideIcon{"isTextBesideIcon"};
        const QString palette{"palette"};
        const QString customPalette{"customPalette"};
    } mSettingsKey;

    QLoggingCategory mLoggingCategory{"sak.settings"};
};

#endif // SAKSETTINGS_H
