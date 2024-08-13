/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSettings>
#include <Qt>

namespace xTools {

class Settings : public QSettings
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(int hdpiPolicy READ hdpiPolicy WRITE setHdpiPolicy NOTIFY hdpiPolicyChanged)
    Q_PROPERTY(bool clearSettings READ clearSettings WRITE setClearSettings NOTIFY clearSettingsChanged)
    // clang-format on

private:
    explicit Settings(QObject *parent = nullptr);

public:
    static Settings *instance();
    Q_INVOKABLE void openSettingsFileDir();

    QString settingsPath();

    int hdpiPolicy();
    void setHdpiPolicy(int policy);

    QString appStyle();
    void setAppStyle(const QString &style);

    QString language();
    void setLanguage(const QString &lan);

    bool clearSettings();
    void setClearSettings(bool clear);

    QString palette();
    void setPalette(const QString &fileName);

    int colorScheme();
    void setColorScheme(const int colorScheme);

signals:
    void hdpiPolicyChanged();
    void uiTypeChanged();
    void clearSettingsChanged();
    void pageIndexChanged();
    void isTextBesideIconChanged();
    void paletteChanged();
    void customPaletteChanged();

public:
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
};

} // namespace xTools
