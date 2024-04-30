/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"
#include "MainWindow.h"
#include "xTools.h"

int main(const int argc, char *argv[])
{
#ifndef X_TOOLS_BUILD_WITH_QMAKE
    Q_INIT_RESOURCE(xToolsCommon);
    Q_INIT_RESOURCE(xToolsTools);
#endif
#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
    Q_INIT_RESOURCE(xToolsPrivateCommon);
#endif
    return xToolsExec<MainWindow, MainWindow, Application>(argc, argv, QString("xTools"));
}
