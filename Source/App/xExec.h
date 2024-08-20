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

void xToolsInitGoogleLogging(char *argv0);
void xToolsShutdownGoogleLogging();
void xToolsQtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void xToolsInitApp(const QString &appName, bool forStore);
void xToolsInstallMessageHandler();
void xToolsTryToClearSettings();
void xToolsInitHdpi();
void xToolsInitAppStyle();
void xToolsDoSomethingBeforeAppCreated(char *argv[], const QString &appName, bool forStore = false);
void xToolsDoSomethingAfterAppExited();
void xToolsTryToRebootApp();

template<typename CentralWidgetT = QWidget,
         typename MainWindowT = xTools::MainWindow,
         typename AppT = xTools::Application>
int xToolsExec(int argc,
               char *argv[],
               const QString &appName,
               const QString &version = QString("0.0.0"),
               const std::function<void(void *, void *)> &doSomethingBeforAppExec = nullptr)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif

    QCoreApplication::setApplicationVersion(version);
    xToolsDoSomethingBeforeAppCreated(argv, appName);

    AppT app(argc, argv);
    //TODO:qt6
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStyleHints *styleHints = QApplication::styleHints();
    styleHints->setColorScheme(Qt::ColorScheme::Dark);
#endif
    const QString dtStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    xTools::Settings::instance()->setValue("startUpTime", dtStr);

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    auto &styleSheetManager = xTools::StyleSheetManager::singleton();
    const QString styleSheet = styleSheetManager.styleSheet();
    if (!styleSheet.isEmpty() && !styleSheetManager.currentTheme().isEmpty()) {
        app.setStyleSheet(styleSheet);
    }
#else
    xToolsInitAppStyle();
#endif

    QWidget *ui;
    if (std::is_same<MainWindowT, CentralWidgetT>::value) {
        auto widget = new CentralWidgetT();
        ui = widget;
    } else {
        auto mainWindow = new MainWindowT();
        auto centralWidget = new CentralWidgetT(mainWindow);
        mainWindow->setWindowTitle(appName);
        mainWindow->setCentralWidget(centralWidget);
        ui = mainWindow;
    }

    QSplashScreen &splashScreen = app.splashScreen();
    splashScreen.finish(ui);
    ui->show();
    ui->resize(static_cast<int>(static_cast<qreal>(ui->height()) * 1.732), ui->height());
    xTools::Application::moveToScreenCenter(ui);
    qInfo() << "The size of window is" << ui->size();

    if (doSomethingBeforAppExec) {
        doSomethingBeforAppExec(ui, &app);
    }
    const int ret = app.exec();
    xToolsDoSomethingAfterAppExited();
    return ret;
}
