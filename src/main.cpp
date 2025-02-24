/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"
#include "common/xtools.h"
#include "mainwindow.h"

int main(const int argc, char *argv[])
{
    Application app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
