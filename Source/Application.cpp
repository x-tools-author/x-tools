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

#include "MainWindow.h"
#include "xToolsSettings.h"

#ifdef Q_OS_WIN
#include "SystemTrayIcon.h"
#endif

Application::Application(int argc, char** argv)
    : xToolsApplication(argc, argv)
{
    // Setup ui language.
    QString language = xToolsSettings::instance()->language();
    xToolsApplication::setupLanguage(language, m_translatorPrefix);
    showSplashScreenMessage(tr("Initializing main window..."));

    auto mainWindow = new MainWindow();
    m_splashScreen.finish(mainWindow);
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
        qInfo() << "The screen is too small.";
    } else {
        mainWindow->move((screenRect.width() - mainWindow->width()) / 2,
                         (screenRect.height() - mainWindow->height()) / 2);
    }
    showSplashScreenMessage(tr("Finished..."));

    QString msg = QString("The size of main window is: %1x%2")
                      .arg(mainWindow->width())
                      .arg(mainWindow->height());
    qInfo() << qPrintable(msg);
}

void Application::setupLanguage(const QString &language)
{
    xToolsApplication::setupLanguage(language);
    xToolsApplication::setupLanguage(language, m_translatorPrefix);
}
