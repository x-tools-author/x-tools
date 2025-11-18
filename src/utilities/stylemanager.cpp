/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "stylemanager.h"
#include "stylemanager_p.h"

namespace xTools {

StyleManager::StyleManager(QObject* parent)
    : QObject(*new StyleManagerPrivate, parent)
{}

StyleManager::~StyleManager() {}

StyleManager& StyleManager::singleton()
{
    static StyleManager instance;
    return instance;
}

void StyleManager::setupAppStyle()
{
    Q_D(StyleManager);
    d->setupAppStyle();
}

void StyleManager::setupSettings(QSettings* settings)
{
    Q_D(StyleManager);
    if (!d->settings) {
        d->settings = settings;
        d->initializeStyleMenu();
    }
}

QMenu* StyleManager::styleMenu()
{
    Q_D(StyleManager);
    return d->styleMenu;
}

} // namespace xTools