/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <functional>
#include <type_traits>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStyle>
#include <QStyleHints>

#include "./Application.h"
#include "./MainWindow.h"
#include "./Settings.h"

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "StyleSheetManager.h"
#endif

static void xInitResources()
{
    Q_INIT_RESOURCE(xTools);
}

namespace xTools {

static void (*gOutputLog2Ui)(QtMsgType, const QMessageLogContext &, const QString &){nullptr};

void initGoogleLogging(char *argv0);
void shutdownGoogleLogging();
void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void initApp(const QString &appName, bool forStore);
void installMessageHandler();
void tryToClearSettings();
void initHdpi();
void initAppStyle();
void doSomethingBeforeAppCreated(char *argv[], const QString &appName, bool forStore = false);
void doSomethingAfterAppExited();
void tryToRebootApp();

} // namespace xTools
