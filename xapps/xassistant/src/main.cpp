/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"
#include "xassistant.h"

#if X_ENABLE_HID
#include <hidapi.h>
#endif

int main(int argc, char *argv[])
{
    Application::setOrganizationName("xAssistant");
    Application::setApplicationName("xAssistant");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);
    app.setApplicationVersion(X_ASSISTANT_VERSION);
    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

#if X_ENABLE_HID
    hid_init();
#endif

    xAssistant window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.load();
    window.showLiteMode();
#if 0
    window.resize(1366, 768);
#else
    window.setFixedSize(1366, 768);
    window.setMinimumHeight(0);
    window.setMinimumWidth(0);
    window.setMaximumHeight(QWIDGETSIZE_MAX);
    window.setMaximumWidth(QWIDGETSIZE_MAX);
#endif
    window.show();
    window.moveToCenter();

    int ret = app.exec();
#if X_ENABLE_HID
    hid_exit();
#endif
    Application::uninstallLog();
    return ret;
}
