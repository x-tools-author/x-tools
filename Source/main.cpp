/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"
#include "xTools.h"

int main(const int argc, char *argv[])
{
    Q_INIT_RESOURCE(xToolsCommon);
    QString appName = QString("xTools");
#ifdef X_TOOLS_BUILD_FOR_STORE
    appName += QObject::tr("(Store)");
#endif
    sakDoSomethingBeforeAppCreated(argv, appName);
    Application app(argc, argv);
    int ret = Application::exec();

    sakDoSomethingAfterAppExited();

    return ret;
}
