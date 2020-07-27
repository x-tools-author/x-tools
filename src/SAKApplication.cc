/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QRect>
#include <QTimer>
#include <QScreen>
#include <QAction>
#include <QSettings>
#include <QTextCursor>
#include <QTranslator>
#include <QDesktopWidget>

#include "SAK.hh"
#include "SAKSettings.hh"
#include "SAKSettings.hh"
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"

SAKApplication::SAKApplication(int argc, char **argv)
    :QApplication (argc, argv)
    ,mMainWindow(Q_NULLPTR)
{
    setApplicationName(QString("QtSwissArmyKnife"));
    /// @brief 率先显示启动页面
    SAKSplashScreen *splashScreen = SAKSplashScreen::instance();
    splashScreen->show();
    processEvents();

    /// @brief 初始化全部变量
    SAKSettings::instance();
#if 0
    SAKSqlDatabase::instance();
#endif

    /// @brief 设置软件版本，SAK_VERSION在SAKCommon中定义
#ifndef SAK_VERSION
    setApplicationVersion(QString("0.0.0"));
#else
    setApplicationVersion(SAK_VERSION);
#endif

    /// @brief 安装语言包，必须在构造界面控件之前调用，否则短语翻译不生效
    installLanguage();

    /// @brief 注册表选项
    setOrganizationName(QString("Qter"));
    setOrganizationDomain(QString("IT"));
    setApplicationName(QString("QtSwissArmyKnife"));

    /// @brief 实例化主窗口
    mMainWindow = new SAKMainWindow;
    mMainWindow->show();
    splashScreen->finish(mMainWindow);

    /// @brief 窗口居中显示
    QDesktopWidget *desktop = QApplication::desktop();
    int currentScreen = desktop->screenNumber(mMainWindow);
    QList<QScreen*> screenList = QGuiApplication::screens();
    QScreen *screen = screenList.at(currentScreen);
    mMainWindow->move((screen->geometry().width() - mMainWindow->width())/2, (screen->geometry().height() - mMainWindow->height())/2);
}

SAKApplication::~SAKApplication()
{
    delete mMainWindow;
}

void SAKApplication::installLanguage()
{
    QString language = SAKSettings::instance()->language();
    QString qmName;
    if (language.isEmpty()){
        if (QLocale().country() == QLocale::China){
            qmName = QString("zh_CN");
        }else{
            qmName = QString("en");
        }
    }else{
        qmName = language.split('-').first();
    }

    mQtBaseTranslator.load(QString(":/translations/qt/qtbase_%1.qm").arg(qmName));
    qApp->installTranslator(&mQtBaseTranslator);

    mQtTranslator.load(QString(":/translations/qt/qt_%1.qm").arg(qmName));
    qApp->installTranslator(&mQtTranslator);

    mSakTranslator.load(QString(":/translations/sak/SAK_%1.qm").arg(qmName));
    qApp->installTranslator(&mSakTranslator);
}

SAKMainWindow *SAKApplication::mainWindow()
{
    return mMainWindow;
}
