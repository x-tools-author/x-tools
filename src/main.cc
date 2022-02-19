/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
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
    int exitCode = 0;
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
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


        // 初始化系统图标
        SAKSystemTrayIcon *systemTrayIcon = new SAKSystemTrayIcon(qApp);
        QObject::connect(systemTrayIcon, &SAKSystemTrayIcon::invokeExit,
                         qApp, [=](){mainWindow->close();});
        QObject::connect(systemTrayIcon,
                         &SAKSystemTrayIcon::invokeShowMainWindow,
                         qApp, [=](){mainWindow->show();});
        systemTrayIcon->show();


        // 将窗口移动至屏幕中心
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


        // 主窗口完成显示后关闭启动页面
        QSplashScreen *splashScreen = app.splashScreen();
        splashScreen->finish(mainWindow);


        // 退出代码等于SAK_REBOOT_CODE(即1314)，软件将重新启动。
        exitCode = app.exec();
#ifdef SAK_REBOOT_CODE
    }while (exitCode == SAK_REBOOT_CODE);
#else
    }while (exitCode == 1314);
#endif
    return exitCode;
}
