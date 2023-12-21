/***************************************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakapplication.h"

#include <QFile>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QSplashScreen>
#include <QTextCursor>
#include <QTranslator>

#include "sakdatastructure.h"
#include "sakmainwindow.h"
#include "saksettings.h"
#include "saktranslator.h"

#ifdef Q_OS_WIN
#include "saksystemtrayicon.h"
#endif

SAKApplication::SAKApplication(int argc, char** argv)
    : QApplication(argc, argv)
{
    // It can avoid app crash in this way to show a splashScreen. If you create a QSplashScreen and
    // show it in the main function, app will crash(test on Ubuntu 16.04). Of course, it is because
    // that I use a wrong way, also, it could be a bug of Qt.
    QPixmap pixmap(":/resources/images/StartUi.jpg");
    m_splashScreen = new QSplashScreen(pixmap);
    showSplashScreenMessage(tr("Initializing..."));
    m_splashScreen->show();
    processEvents();

    // Palette
    int ret = SAKSettings::instance()->palette();
    if ((ret == SAKDataStructure::PaletteDark) || (ret == SAKDataStructure::PaletteLight)) {
        QString fileName = (ret == SAKDataStructure::PaletteLight
                                ? ":/resources/palette/SAKAppPaletteLight"
                                : ":/resources/palette/SAKAppPaletteDark");
        setupPalette(fileName);
    } else {
        QString customPalette = SAKSettings::instance()->customPalette();
        if (customPalette.isEmpty()) {
            qInfo() << "current palette: system";
        } else {
            setupPalette(customPalette);
        }
    }

    // Setup ui language.
    QString language = SAKSettings::instance()->language();
    SAKTranslator::instance()->setupLanguage(language);
    showSplashScreenMessage(tr("Initializing main window..."));

    auto mainWindow = new SAKMainWindow();
    m_splashScreen->finish(mainWindow);
    QObject::connect(this,
                     &SAKApplication::activeMainWindow,
                     mainWindow,
                     &SAKMainWindow::activateWindow);
    mainWindow->show();

#ifdef Q_OS_WIN
    // Setup system tray icon.
    auto systemTrayIcon = new SAKSystemTrayIcon(this);
    QObject::connect(systemTrayIcon, &SAKSystemTrayIcon::invokeExit, this, [=]() {
        mainWindow->close();
    });
    QObject::connect(systemTrayIcon, &SAKSystemTrayIcon::invokeShowMainWindow, this, [=]() {
        mainWindow->show();
    });
    systemTrayIcon->show();
#endif

    // Move the window to the screen central.
#ifndef Q_OS_ANDROID
    mainWindow->resize(int(double(mainWindow->height()) * 1.732), mainWindow->height());
#endif
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    bool tooWidth = (mainWindow->width() > screenRect.width());
    bool tooHeight = (mainWindow->height() > screenRect.height());
    if (tooWidth || tooHeight) {
        mainWindow->showMaximized();
        qInfo() << "too small screen";
    } else {
        mainWindow->move((screenRect.width() - mainWindow->width()) / 2,
                         (screenRect.height() - mainWindow->height()) / 2);
    }
    showSplashScreenMessage(tr("Finished..."));

    QString msg = QString("the size of main window is: %1x%2")
                      .arg(mainWindow->width())
                      .arg(mainWindow->height());
    qInfo() << qPrintable(msg);
}

void SAKApplication::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen->showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void SAKApplication::setupPalette(const QString& fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QDataStream out(&file);
        QPalette p;
        out >> p;
        file.close();
        setPalette(p);
        qInfo() << "current palette:" << fileName;
    } else {
        qWarning() << "open palette file error:" << file.errorString();
    }
}
