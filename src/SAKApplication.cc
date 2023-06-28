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
#include "SAKDataStructure.hh"
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

    // Palete
    int ret = SAKSettings::instance()->palette();
    if ((ret == SAKDataStructure::PaletteDark)
        || (ret == SAKDataStructure::PaletteLight)) {
        QString fileName = (ret == SAKDataStructure::PaletteLight
                                ? ":/resources/palette/SAKAppPaletteLight"
                                : ":/resources/palette/SAKAppPaletteDark");
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QDataStream out(&file);
            QPalette p;
            out >> p;
            file.close();
            setPalette(p);
            qCInfo(mLoggingCategory) << "current palete:" << fileName;
        } else {
            qCWarning(mLoggingCategory) << "open palette file error:"
                                        << file.errorString();
        }
    } else {
        QString customPalette = SAKSettings::instance()->customPalette();
        QFile file(customPalette);
        if (file.open(QFile::ReadOnly)) {
            QDataStream out(&file);
            QPalette p;
            out >> p;
            file.close();
            setPalette(p);
            qCInfo(mLoggingCategory) << "current palete:" << customPalette;
        } else {
            qCWarning(mLoggingCategory) << "open palette file error:"
                                        << file.errorString();
        }
        qCInfo(mLoggingCategory) << "current palete: system";
    }

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
    QString msg = QString("the size of main window is: %1x%2")
                      .arg(mainWindow->width()).arg(mainWindow->height());
    qCInfo(mLoggingCategory) << qPrintable(msg);
}

SAKApplication::~SAKApplication()
{

}

QSplashScreen *SAKApplication::splashScreen()
{
    return mSplashScreen;
}

void SAKApplication::showSplashScreenMessage(QString msg)
{
    mSplashScreen->showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}
