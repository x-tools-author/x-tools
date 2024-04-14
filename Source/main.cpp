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
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif
    Q_INIT_RESOURCE(xToolsCommon);
#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
    Q_INIT_RESOURCE(xToolsPrivateCommon);
#endif
    return xToolsExec<MainWindow, MainWindow, Application>(argc, argv, QString("xTools"));
}
