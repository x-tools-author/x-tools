/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDateTime>

#include "tools/filecheck/filecheckassistant.h"
#include "x/xapp.h"
#include "x/xui.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("xTools");
    QApplication::setApplicationName("xHash");
    QApplication::setApplicationVersion("1.0.0");

    xApp app(argc, argv);
    auto settings = app.settings();
    settings->setValue("Application/boost", QDateTime::currentDateTime().toString(Qt::ISODate));

    app.setupHdpi();
    app.setupTheme();
    app.setupLanguage();

    xUi window;
    FileCheckAssistant *fileCheckAssistant = new FileCheckAssistant(&window);
    window.setCentralWidget(fileCheckAssistant);
    window.setWindowIcon(QIcon(":/res/icons/logo.svg"));
    auto flags = window.windowFlags();
    flags &= ~Qt::WindowMinMaxButtonsHint;
    window.setWindowFlags(Qt::WindowFlags(flags));
    window.setMinimumWidth(520);
    window.show();
    window.moveToCenter();

    return app.exec();
}