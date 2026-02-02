/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDebug>

#if X_ENABLE_HID
#include <hidapi.h>
#endif

#if X_ENABLE_X_COAP
#include <coap3/coap.h>
#endif

#if X_ENABLE_SINGLE_APPLICATION
#include <singleapplication.h>
#endif

#include "application.h"
#include "log/log.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
#if X_ENABLE_LOG
    qInstallMessageHandler(xLog::Log::messageHandler);
#endif
#if X_ENABLE_HID
    hid_init();
#endif
#if X_ENABLE_X_COAP
    coap_startup();
    coap_set_log_level(COAP_LOG_OSCORE);
#endif

    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);

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
    window.show();
    window.moveToCenter();
    window.load();

#if X_ENABLE_SINGLE_APPLICATION
    QObject::connect(&sApp, &SingleApplication::instanceStarted, &window, [&window]() {
        window.show();
        window.raise();
        window.activateWindow();
        QApplication::beep();
    });
#endif

    const int ret = Application::exec();
    Application::uninstallLog();
#if X_ENABLE_HID
    hid_exit();
#endif
#if X_ENABLE_X_COAP
    coap_cleanup();
#endif
    qInfo() << "Application exited with code:" << ret;
    return ret;
}
