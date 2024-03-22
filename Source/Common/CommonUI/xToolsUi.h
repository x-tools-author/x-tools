/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>

#include "xTools.h"
#include "xToolsMainWindow.h"

template<typename UiT, typename MainWindowT, typename AppT>
int xToolsExec(int argc, char* argv[], const QString& appName, bool usingCommonMainWindow = true)
{
    sakDoSomethingBeforeAppCreated(argv, appName);

    AppT app(argc, argv);
    if (usingCommonMainWindow) {
        MainWindowT* mainWindow = new MainWindowT();
        UiT* centralWidget = new UiT(mainWindow);
        mainWindow->setWindowTitle(appName);
        mainWindow->setCentralWidget(centralWidget);
        mainWindow->show();
        mainWindow->resize(int(qreal(mainWindow->height()) * 1.732), mainWindow->height());
    } else {
        UiT* widget = new UiT();
        widget->show();
        widget->resize(int(qreal(widget->height()) * 1.732), widget->height());
    }

    int ret = app.exec();
    sakDoSomethingAfterAppExited();
    return ret;
}

template<typename T>
int xToolsExec(int argc, char* argv[], const QString& appName, bool usingCommonMainWindow = true)
{
    return xToolsExec<T, xToolsMainWindow, QApplication>(argc, argv, appName, usingCommonMainWindow);
}
