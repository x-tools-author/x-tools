/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDebug>
#include <QMessageBox>

#if defined(X_ENABLE_HID)
#include <hidapi.h>
#endif

#include "application.h"
#include "mainwindow.h"

#if defined(X_ENABLE_SINGLE_APPLICATION)
#include "singleapplication.h"
#endif

int main(int argc, char *argv[])
{
    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);

#if defined(X_ENABLE_SINGLE_APPLICATION)
    SingleApplication sApp(argc, argv);
    if (sApp.isSecondary()) {
        return 0;
    }
#endif

    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

#if defined(X_ENABLE_HID)
    hid_init();
#endif

    MainWindow window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.resize(1280, 720);
    window.show();
    window.load();
    window.moveToCenter();

#if defined(X_ENABLE_SINGLE_APPLICATION)
    QObject::connect(&sApp, &SingleApplication::instanceStarted, &window, [&window]() {
        window.show();
        window.raise();
        window.activateWindow();
        QApplication::beep();
    });
#endif

    int ret = app.exec();

#if defined(X_ENABLE_HID)
    hid_exit();
#endif

    Application::uninstallLog();
    qInfo() << "Application exited with code:" << ret;
    return ret;
}