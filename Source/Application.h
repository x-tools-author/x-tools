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
#include "xApp/Source/Application.h"
#else
#include "App/Application.h"
#endif

#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
class Application : public xApp::Application
#else
class Application : public xTools::Application
#endif
{
    Q_OBJECT
public:
    explicit Application(int argc, char **argv);
    ~Application() override = default;
};
