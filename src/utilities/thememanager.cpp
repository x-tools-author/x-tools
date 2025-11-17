/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "thememanager.h"
#include "thememanager_p.h"

ThemeManager::ThemeManager(QObject *parent)
    : QObject(*new ThemeManagerPrivate, parent)
{}

ThemeManager::~ThemeManager() {}

ThemeManager &ThemeManager::singleton()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setupSettings(QSettings *settings)
{
    Q_D(ThemeManager);
    d->settings = settings;
}

void ThemeManager::updateApplicationColor()
{
    Q_D(ThemeManager);
    d->updateApplicationColor();
}

void ThemeManager::updateWindowCaptionColor()
{
    Q_D(ThemeManager);
    d->updateWindowCaption();
}

void ThemeManager::updateWindowCaptionColor(QWidget *widget) {}

QMenu *ThemeManager::themeMenu()
{
    Q_D(ThemeManager);
    return d->themeMenu();
}