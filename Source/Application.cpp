/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"

#include <QFile>

#include "App/Settings.h"

Application::Application(int argc, char **argv)
#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
    : xToolsPrivateApplication(argc, argv)
#else
    : xTools::Application(argc, argv)
#endif
{
    // Setup ui language.
    const QString language = xTools::Settings::instance()->language();
    setupLanguageWithPrefix(language, m_translatorPrefix);
    setupLanguageWithPrefix(language, m_translatorToolsPrefix);
    showSplashScreenMessage(tr("Initializing main window..."));
}

void Application::setupLanguage(const QString &language)
{
    xTools::Application::setupLanguage(language);
    setupLanguageWithPrefix(language, m_translatorPrefix);
    setupLanguageWithPrefix(language, m_translatorToolsPrefix);
}
