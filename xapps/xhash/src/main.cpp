/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDateTime>

#include "common/xapp.h"
#include "common/xui.h"
#include "tools/filecheck/filecheckassistant.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("xTools");
    QApplication::setApplicationName("xHash");
    QApplication::setApplicationVersion(QString(X_HASH_VERSION));

    xApp app(argc, argv);
    auto settings = app.settings();
    settings->setValue("Application/boost", QDateTime::currentDateTime().toString(Qt::ISODate));

    app.setupHdpi();
    app.setupColorScheme();
    app.setupLanguage();

    xUi window;
    FileCheckAssistant *fileCheckAssistant = new FileCheckAssistant(&window);
    window.setCentralWidget(fileCheckAssistant);
    window.setWindowIcon(QIcon(":/res/icons/logo.svg"));
    Qt::WindowFlags flags = window.windowFlags();
    flags &= ~Qt::WindowMinMaxButtonsHint;
    window.setWindowFlags(flags);
    window.setMinimumWidth(520);
    window.show();
    window.moveToCenter();

    return app.exec();
}