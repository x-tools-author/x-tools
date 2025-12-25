/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "./application.h"
#include "./mainwindow.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("xTools");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}