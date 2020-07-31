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
    /// @brief 当退出代码与SAK_REBOOT_CODE相等时，重新初始化软件（相当于重启）
    do {
        SAKApplication app(argc, argv);
        /// @brief 检测是否存在已运行的实例，如果存在，终止本次启动,同时激活已启动的程序
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
        exitCode = app.exec();
    }while (exitCode == SAK_REBOOT_CODE);

    return exitCode;
}
