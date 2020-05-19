/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKApplication.hh"
#include "SAKConsoleManager.hh"

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);
#endif
    SAKApplication app(argc, argv);

#ifndef QT_NO_DEBUG
    /// @brief 消息重定位，发布版本有效
    SAKConsoleManager *consoleManager = SAKConsoleManager::instance();
    qInstallMessageHandler(SAKConsoleManager::consoleOutput);
#endif

    /// @brief 开始加载ui
    app.setupUi();

    return app.exec();
#ifndef QT_NO_DEBUG
    consoleManager->deleteLater();
#endif
}
