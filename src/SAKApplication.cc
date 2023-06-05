/******************************************************************************
 * Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDir>
#include <QUrl>
#include <QRect>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QAction>
#include <QDialog>
#include <QScreen>
#include <QDateTime>
#include <QSettings>
#include <QSizePolicy>
#include <QGridLayout>
#include <QTextCursor>
#include <QTranslator>
#include <QPushButton>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QStandardPaths>

#include "SAKSettings.hh"
#include "SAKTranslator.hh"
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSystemTrayIcon.hh"

QDate buildDate = QLocale(QLocale::English).toDate(
            QString(__DATE__).replace("  ", " 0"),
            "MMM dd yyyy");
QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss");
SAKApplication::SAKApplication(int argc, char **argv)
    :QApplication (argc, argv)
{
    // It can avoid app crash in this way to show a splashScreen.
    // If you new a QSplashScreen and show it in the main function,
    // app will crash(test on Ubuntu 16.04).
    // Of course, it is because that I use a wrong way,
    // also, it could be a bug of Qt.
    QPixmap pixmap(":/resources/images/StartUi.jpg");
    mSplashScreen = new QSplashScreen(pixmap);
    showSplashScreenMessage(tr("Initializing..."));
    mSplashScreen->show();
    processEvents();

    // Setup ui language.
    QString language = SAKSettings::instance()->language();
    SAKTranslator::instance()->setupLanguage(language);
    showSplashScreenMessage(QObject::tr("Initializing main window..."));

    SAKMainWindow *mainWindow = new SAKMainWindow();
    QObject::connect(this, &SAKApplication::activeMainWindow,
                     mainWindow, &SAKMainWindow::activateWindow);
    mainWindow->show();

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
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    bool tooWidth = (mainWindow->width() > screenRect.width());
    bool tooHeight = (mainWindow->height() > screenRect.height());

#ifndef Q_OS_ANDROID
    if (!(tooHeight || tooWidth)) {
        mainWindow->resize(mainWindow->height() * 1.732, mainWindow->height());
    }
#endif

    if (tooWidth && tooHeight) {
        mainWindow->showMaximized();
    } else {
        mainWindow->move((screenRect.width() - mainWindow->width())/2,
                         (screenRect.height() - mainWindow->height())/2);
    }
    showSplashScreenMessage(tr("Finished..."));


    // Close splash screen after main window showed.
    QSplashScreen *ss = splashScreen();
    ss->finish(mainWindow);
}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::installLanguage()
{
    QString key = SAKSettings::instance()->language();
    Q_UNUSED(key)
//    QString language = mSettings->value(key).toString();
//    QString qmName;
//    if (language.isEmpty()) {
//        if (QLocale().country() == QLocale::China){
//            qmName = QString("zh_CN");
//            mSettings->setValue(mSettingsKeyContext.language,
//                                QString("zh_CN-简体中文"));
//        } else {
//            qmName = QString("en");
//            mSettings->setValue(mSettingsKeyContext.language,
//                                QString("en-English"));
//        }
//    } else {
//        qmName = language.split('-').first();
//    }

//    QString qm = QString(":/translations/qt/qtbase_%1.qm").arg(qmName);
//    auto ret = mQtBaseTranslator.load(qm);
//    Q_UNUSED(ret);
//    qApp->installTranslator(&mQtBaseTranslator);

//    qm = QString(":/translations/qt/qt_%1.qm").arg(qmName);
//    ret = mQtTranslator.load(qm);
//    Q_UNUSED(ret);
//    qApp->installTranslator(&mQtTranslator);

//    qm = QString(":/translations/sak/SAK_%1.qm").arg(qmName);
//    ret = mSakTranslator.load(qm);
//    Q_UNUSED(ret);
//    qApp->installTranslator(&mSakTranslator);
}

const QDate *SAKApplication::buildDate()
{
    return &::buildDate;
}

const QTime* SAKApplication::buildTime()
{
    return &::buildTime;
}

QString SAKApplication::dataPath()
{
    QString path = SAKSettings::instance()->fileName();
    QStringList temp = path.split('/');
    path = path.remove(temp.last());

    return path;
}

QSplashScreen *SAKApplication::splashScreen()
{
    return mSplashScreen;
}

void SAKApplication::showSplashScreenMessage(QString msg)
{
    mSplashScreen->showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}
