/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hdpimanager.h"
#include "hdpimanager_p.h"

namespace xTools {

HdpiManager::HdpiManager(QObject* parent)
    : QObject(*new HdpiManagerPrivate(), parent)
{}

HdpiManager::~HdpiManager() {}

HdpiManager& HdpiManager::singleton()
{
    static HdpiManager instance;
    return instance;
}

void HdpiManager::setupSettings(QSettings* settings)
{
    Q_D(HdpiManager);
    d->settings = settings;
}

void HdpiManager::setupHdpi()
{
    Q_D(HdpiManager);
    d->setupHdpi();
}

QMenu* HdpiManager::hdpiMenu()
{
    Q_D(HdpiManager);
    if (!d->hdpiMenu) {
        d->initHdpiMenu();
    }

    return d->hdpiMenu;
}

} // namespace xTools