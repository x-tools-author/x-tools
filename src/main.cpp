/***************************************************************************************************
 * Copyright 2018-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDebug>
#include <QScreen>

#if X_ENABLE_SINGLE_APPLICATION
#include <singleapplication.h>
#endif

#if X_ENABLE_LOG
#include "log/log.h"
#endif

#include "application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
#if X_ENABLE_LOG
    qInstallMessageHandler(xLog::Log::messageHandler);
#endif

    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::setupHdpi();
    Application app(argc, argv);
    app.setCustomSplashScreen(QPixmap(":/res/images/splash_screen.png"));

#if X_ENABLE_SINGLE_APPLICATION
    SingleApplication sApp(argc, argv);
    if (sApp.isSecondary()) {
        return 0;
    }
#endif

    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

    MainWindow window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.resize(1366, 768);
    window.load();
    window.show();
    window.moveToCenter();

#if X_ENABLE_SINGLE_APPLICATION
    QObject::connect(&sApp, &SingleApplication::instanceStarted, &window, [&window]() {
        window.show();
        window.raise();
        window.activateWindow();
        QApplication::beep();
    });
#endif

    return Application::exec();
}
