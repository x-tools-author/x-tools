/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>

#include "themeiconview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ThemeIconView view;
    view.adjustSize();
    view.show();
    return app.exec();
}