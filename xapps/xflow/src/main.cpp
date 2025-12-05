/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xflow/src/application.h"
#include "xflow/src/mainwindow.h"

#if X_ENABLE_HID
#include <hidapi.h>
#endif

int main(int argc, char *argv[])
{
    Application::setOrganizationName("xFlow");
    Application::setApplicationName("xFlow");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);
    app.setApplicationVersion(X_LATEST_GIT_TAG);
    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

#if X_ENABLE_HID
    hid_init();
#endif

    MainWindow window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.show();
    window.resize(1366, 768);
    window.moveToCenter();
    window.load();

    int ret = app.exec();
#if X_ENABLE_HID
    hid_exit();
#endif
    Application::uninstallLog();
    return ret;
}