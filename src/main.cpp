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

int main(int argc, char *argv[])
{
    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);
    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

    MainWindow window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.resize(620 * 1.732, 620);
    window.show();
    window.load();
    window.moveToCenter();

    int ret = app.exec();
    Application::uninstallLog();
    return ret;
}
