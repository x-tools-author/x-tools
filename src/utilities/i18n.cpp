/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "i18n.h"
#include "i18n_p.h"

namespace xTools {

I18n::I18n(QObject* parent)
    : QObject(*new I18nPrivate(), parent)
{}

I18n::~I18n() {}

I18n& I18n::singleton()
{
    static I18n instance;
    return instance;
}

QMenu* I18n::languageMenu()
{
    Q_D(I18n);
    return d->languageMenu;
}

void I18n::setupSettings(QSettings* settings)
{
    Q_D(I18n);
    d->settings = settings;
    d->initLanguageMenu();
}

void I18n::setupLanguage()
{
    Q_D(I18n);
    d->setupLanguage();
}

} // namespace xTools