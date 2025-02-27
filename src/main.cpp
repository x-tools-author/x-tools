/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"
#include "mainwindow.h"

int main(const int argc, char *argv[])
{
    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application::setupAppStyle();
    Application app(argc, argv);
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.execMs(200);
    app.showSplashScreenMessage(QObject::tr("Setup language..."));
    app.setupLanguage();

    QSplashScreen *splash = Application::splashScreen();
    MainWindow window;
    splash->finish(&window);
    window.resize(620 * 1.732, 620);
    window.show();

    int ret = app.exec();
    Application::uninstallLog();
    return ret;
}
