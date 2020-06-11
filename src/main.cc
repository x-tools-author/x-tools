/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);
#endif
    SAKApplication app(argc, argv);

    /// @brief 在Deepin 15系统中，在main函数中创建SAKMainWindow会导致程序崩溃(Qt5.12.8)
#if 0
    SAKMainWindow mainWindow;
    mainWindow.show();
#endif

    return app.exec();
}
