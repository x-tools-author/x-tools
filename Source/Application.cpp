/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"

#include <QFile>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QSplashScreen>
#include <QTextCursor>
#include <QTranslator>

#include "sakdatastructure.h"
#include "MainWindow.h"
#include "saksettings.h"
#include "saktranslator.h"

#ifdef Q_OS_WIN
#include "SystemTrayIcon.h"
#endif

Application::Application(int argc, char** argv)
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
    
    auto mainWindow = new MainWindow();
    m_splashScreen->finish(mainWindow);
    QObject::connect(this,
                     &Application::activeMainWindow,
                     mainWindow,
                     &MainWindow::activateWindow);
    mainWindow->show();

#ifdef Q_OS_WIN
    // Setup system tray icon.
    auto systemTrayIcon = new SystemTrayIcon(this);
    QObject::connect(systemTrayIcon, &SystemTrayIcon::invokeExit, this, [=]() {
        mainWindow->close();
    });
    QObject::connect(systemTrayIcon, &SystemTrayIcon::invokeShowMainWindow, this, [=]() {
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

void Application::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen->showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void Application::setupPalette(const QString& fileName)
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
