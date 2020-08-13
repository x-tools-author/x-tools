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

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
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
        QObject::connect(&controller, &SAKSingletonController::showMainWindowInstanceRequest, app.mainWindow(), &SAKMainWindow::show);
        QObject::connect(&controller, &SAKSingletonController::showMainWindowInstanceRequest, app.mainWindow(), &SAKMainWindow::activateWindow);
        if (controller.isInstanceExist()){
            SAKSingletonErrorDialog dialog;
            QApplication::beep();
            dialog.exec();
            controller.setFlag();
            return -1024;
        }

        // If exit code is SAK_REBOOT_CODE(1314), The application will reboot.
        exitCode = app.exec();
    }while (exitCode == SAK_REBOOT_CODE);

    return exitCode;
}
