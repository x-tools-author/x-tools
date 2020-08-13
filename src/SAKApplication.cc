/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QRect>
#include <QFile>
#include <QTimer>
#include <QDebug>
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
#include "SAKSqlDatabase.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"

SAKApplication::SAKApplication(int argc, char **argv)
    :QApplication (argc, argv)
    ,mMainWindow(Q_NULLPTR)
{
    // Initialize some information about application.
    setOrganizationName(QString("Qter"));
    setOrganizationDomain(QString("IT"));
    setApplicationName(QString("QtSwissArmyKnife"));

    // Setup language packet at first.
    installLanguage();

    // Show a splash screen.
    SAKSplashScreen *splashScreen = SAKSplashScreen::instance();
    splashScreen->show();
    processEvents();

    // Remove settings files
    QSettings settings(SAKSettings::fullPath(), QSettings::IniFormat);
    if (settings.value(SAKMainWindow::settingKeyClearConfiguration()).toBool()){
        settings.setValue(SAKMainWindow::settingKeyClearConfiguration(), QVariant::fromValue(false));

        if (QFile::remove(SAKSettings::fullPath())){
            qInfo() << "Remove settings file successfully!";
        }

        QFile databaseFile(SAKSqlDatabase::fullPath());
        if (databaseFile.remove()){
            qInfo() << "Remove database successfully!";
        }else{
            qWarning() << "Remove database failed: " << databaseFile.errorString();
        }
    }


    // Initialize some global variables.
    SAKSettings::instance();
    SAKSqlDatabase::instance();

    // Set application version, if micro SAK_VERSION is not defined, the application version is "0.0.0"
#ifndef SAK_VERSION
    setApplicationVersion(QString("0.0.0"));
#else
    setApplicationVersion(SAK_VERSION);
#endif

    // There is bug: the application will crash if create and show a main window in the main().
    // the bug is appear on linux platform only.
    splashScreen->setMessage(tr("Initializing main window..."));
    mMainWindow = new SAKMainWindow;
    mMainWindow->show();

    // Move the main window to the central of desktop.
    QDesktopWidget *desktop = QApplication::desktop();
    int currentScreen = desktop->screenNumber(mMainWindow);
    QList<QScreen*> screenList = QGuiApplication::screens();
    QScreen *screen = screenList.at(currentScreen);
    mMainWindow->move((screen->geometry().width() - mMainWindow->width())/2, (screen->geometry().height() - mMainWindow->height())/2);
    splashScreen->setMessage(tr("Finished..."));
    splashScreen->finish(mMainWindow);
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
