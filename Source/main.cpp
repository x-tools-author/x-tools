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

    QString version = "0.0.0";
#if defined(X_TOOLS_GIT_TAG)
    QString tag = QString(X_TOOLS_GIT_TAG);
    if (tag == QString("continuous")) {
        version = "9.9.9";
    } else {
        tag = tag.remove("v");
        if (tag.split('.').length() == 3) {
            version = tag;
        }
    }
#endif
    return xToolsExec<MainWindow, MainWindow, Application>(argc, argv, QString("xTools"), version);
}
