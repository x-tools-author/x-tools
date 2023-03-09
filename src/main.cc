/******************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
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
#include "SAKSystemTrayIcon.hh"

int main(int argc, char *argv[])
{

#if 0
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
#endif
#endif

    int exitCode = 0;
    do {
        SAKApplication app(argc, argv);
        app.showSplashScreenMessage(
                    QObject::tr("Initializing main window..."));


        SAKMainWindow *mainWindow = new SAKMainWindow(app.settings(),
                                                      app.sqlDatabase());
        QObject::connect(&app, &SAKApplication::activeMainWindow,
                         mainWindow, &SAKMainWindow::activateWindow);
        mainWindow->show();
#ifndef Q_OS_ANDROID
        mainWindow->resize(mainWindow->height() * 1.732,
                           mainWindow->height());
#endif


#ifdef Q_OS_WIN
        // Setup system tray icon.
        SAKSystemTrayIcon *systemTrayIcon = new SAKSystemTrayIcon(qApp);
        QObject::connect(systemTrayIcon, &SAKSystemTrayIcon::invokeExit,
                         qApp, [=](){mainWindow->close();});
        QObject::connect(systemTrayIcon,
                         &SAKSystemTrayIcon::invokeShowMainWindow,
                         qApp, [=](){mainWindow->show();});
        systemTrayIcon->show();
#endif


        // Move the window to the screen central.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QDesktopWidget *desktop = QApplication::desktop();
        int currentScreen = desktop->screenNumber(mainWindow);
        QList<QScreen*> screenList = QGuiApplication::screens();
        QScreen *screen = screenList.at(currentScreen);
        mainWindow->move((screen->geometry().width()
                          - mainWindow->width())/2,
                        (screen->geometry().height()
                         - mainWindow->height())/2);
        app.showSplashScreenMessage(QObject::tr("Finished..."));
#endif


        // Close splash screen after main window showed.
        QSplashScreen *splashScreen = app.splashScreen();
        splashScreen->finish(mainWindow);


        // If the exit code is equal to SAK_REBOOT_CODE, the application will
        // be reboot.
        exitCode = app.exec();
#ifdef SAK_REBOOT_CODE
    }while (exitCode == SAK_REBOOT_CODE);
#else
    }while (exitCode == 1314);
#endif
    return exitCode;
}
