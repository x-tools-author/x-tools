/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKUI_H
#define SAKUI_H

#include <QApplication>

#include "sak.h"
#include "sakcommonmainwindow.h"
#include "saksettings.h"

#define SAK_APP(WidgetType, argc, argv, appName) \
    sakDoSomethingBeforeAppCreated(argv, appName); \
    QApplication app(argc, argv); \
    SAKCommonMainWindow* mainWindow = new SAKCommonMainWindow(); \
    WidgetType* centralWidget = new WidgetType(mainWindow); \
    mainWindow->setWindowTitle(appName); \
    mainWindow->setCentralWidget(centralWidget); \
    if (mainWindow->height() < 400) { \
        mainWindow->setMinimumHeight(400); \
    } \
    mainWindow->resize(int(qreal(mainWindow->height()) * 1.732), mainWindow->height()); \
    mainWindow->show();

#endif // SAKUI_H