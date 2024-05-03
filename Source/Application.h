/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
#include "xToolsPrivateApplication.h"
#else
#include "xToolsApplication.h"
#endif

#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
class Application : public xToolsPrivateApplication
#else
class Application : public xToolsApplication
#endif
{
    Q_OBJECT
public:
    explicit Application(int argc, char **argv);
    ~Application() override = default;

    void setupLanguage(const QString &language) override;

private:
    const QString m_translatorPrefix{"xTools"};
    const QString m_translatorToolsPrefix{"xToolsTools"};
};
