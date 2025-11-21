/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>
#include <QObject>
#include <QSettings>
#include <QWidget>

#define xThemeMgr xTools::ThemeManager::singleton()

namespace xTools {

class ThemeManagerPrivate;
class ThemeManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ThemeManager)
    Q_DISABLE_COPY(ThemeManager)

    ThemeManager(QObject *parent = nullptr);

public:
    ~ThemeManager() override;

    static ThemeManager &singleton();
    void setupSettings(QSettings *settings);
    void updateApplicationColor();
    void updateWindowCaptionColor();
    void updateWindowCaptionColor(QWidget *widget);
    QMenu *themeMenu();

signals:
    void colorSchemeChanged();
};

} // namespace xTools