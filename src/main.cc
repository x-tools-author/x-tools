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
#include <QScreen>
#include <QDesktopWidget>

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"
#include "SAKSingletonController.hh"
#include "SAKSingletonErrorDialog.hh"

int main(int argc, char *argv[])
{
    int exitCode = 0;

    // The application can be reboot.
    do {
        SAKApplication app(argc, argv);

        // The applicatin is singleton application, if there is a running application, the starting will be terminated.
        SAKSingletonController controller;
        if (controller.isInstanceExist()){
            SAKSingletonErrorDialog dialog;
            QApplication::beep();
            dialog.exec();
            controller.setFlag();
            return -1024;
        }

        // Show a splash screen.
        SAKSplashScreen *splashScreen = SAKSplashScreen::instance();
        splashScreen->show();
        app.processEvents();

        // There is bug: the application will crash if create and show a main window in the main().
        // the bug is appear on linux platform only.
        splashScreen->setMessage(QObject::tr("Initializing main window..."));
        SAKMainWindow mainWindow;
        mainWindow.show();

        QObject::connect(&controller, &SAKSingletonController::showMainWindowInstanceRequest, &mainWindow, &SAKMainWindow::show);
        QObject::connect(&controller, &SAKSingletonController::showMainWindowInstanceRequest, &mainWindow, &SAKMainWindow::activateWindow);

        // Move the main window to the central of desktop.
        QDesktopWidget *desktop = QApplication::desktop();
        int currentScreen = desktop->screenNumber(&mainWindow);
        QList<QScreen*> screenList = QGuiApplication::screens();
        QScreen *screen = screenList.at(currentScreen);
        mainWindow.move((screen->geometry().width() - mainWindow.width())/2, (screen->geometry().height() - mainWindow.height())/2);
        splashScreen->setMessage(QObject::tr("Finished..."));
        splashScreen->finish(&mainWindow);

        // If exit code is SAK_REBOOT_CODE(1314), The application will reboot.
        exitCode = app.exec();
        delete screen;
    }while (exitCode == SAK_REBOOT_CODE);

    return exitCode;
}
