/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QScreen>
#include <QPushButton>
#include <QGridLayout>
#include <QSplashScreen>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#endif

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"

int main(int argc, char *argv[])
{
    int exitCode = 0;

    // The application can be reboot.
    do {
        SAKApplication app(argc, argv);
        // Setup main window
        app.showSplashScreenMessage(QObject::tr("Initializing main window..."));
        SAKMainWindow mainWindow;
        QObject::connect(&app, &SAKApplication::activeMainWindow, &mainWindow, &SAKMainWindow::activateWindow);
        mainWindow.show();
#ifndef Q_OS_ANDROID
        // Golden ratio
        mainWindow.resize(mainWindow.height() * 1.732, mainWindow.height());
#endif

        // Move the main window to the central of desktop.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QDesktopWidget *desktop = QApplication::desktop();
        int currentScreen = desktop->screenNumber(&mainWindow);
        QList<QScreen*> screenList = QGuiApplication::screens();
        QScreen *screen = screenList.at(currentScreen);
        mainWindow.move((screen->geometry().width() - mainWindow.width())/2, (screen->geometry().height() - mainWindow.height())/2);
        app.showSplashScreenMessage(QObject::tr("Finished..."));
#endif

        // Close the splash screen.
        QSplashScreen *splashScreen = app.splashScreen();
        splashScreen->finish(&mainWindow);

        // If exit code is SAK_REBOOT_CODE(1314), The application will reboot.
        exitCode = app.exec();
    }while (exitCode == SAK_REBOOT_CODE);

    return exitCode;
}
