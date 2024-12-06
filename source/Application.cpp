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

Application::Application(int argc, char **argv)
#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
    : xApp::Application(argc, argv)
#else
    : xTools::Application(argc, argv)
#endif
{

}
