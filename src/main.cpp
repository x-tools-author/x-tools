/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"
#include "mainwindow.h"

int main(const int argc, char *argv[])
{
    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog();
    Application app(argc, argv);

    MainWindow window;
    window.resize(620 * 1.732, 620);
    window.show();

    int ret = app.exec();
    Application::uninstallLog();
    return ret;
}
