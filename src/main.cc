/******************************************************************************
 * Copyright 2018-2023 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
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

#include <QLocale>
#include <QVariant>
#include <QQuickStyle>
#include <QQmlContext>
#include <QFontDatabase>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSystemTrayIcon.hh"

#include "common/EDBle.hpp"
#include "common/SAKCrcInterface.hh"
#include "common/EDI18N.hpp"
#include "common/SAKSettings.hh"
#include "common/SAKInterface.hh"
#include "common/EDSerialPort.hpp"
#include "common/EDHighlighter.hpp"
#include "common/SAKDataStructure.hh"
#include "common/EDNetworkInterface.hpp"

#include "toolbox/SAKToolBox.hh"

#include "tools/SAKBaseTool.hh"
#include "tools/SAKMaskerTool.hh"
#include "tools/SAKStorerTool.hh"
#include "tools/SAKEmitterTool.hh"
#include "tools/SAKAnalyzerTool.hh"
#include "tools/SAKToolFactory.hh"
#include "tools/SAKUdpClientTool.hh"
#include "tools/SAKUdpServerTool.hh"
#include "tools/SAKTcpClientTool.hh"
#include "tools/SAKTcpServerTool.hh"
#include "tools/SAKPrestorerTool.hh"
#include "tools/SAKResponserTool.hh"
#include "tools/SAKSerialPortTool.hh"
#include "tools/SAKBleCentralTool.hh"
#include "tools/SAKWebSocketClientTool.hh"
#include "tools/SAKWebSocketServerTool.hh"

int main(int argc, char *argv[])
{

#if 0
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
#endif
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif

    // High dpi settings.
    auto edSettings = SAKSettings::instance();
#if 0
    auto policy = edSettings->value("highDpiScaleFactorRoundingPolicy").toInt();
    auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
#endif
    // Language setttings.
    auto language = edSettings->value("language").toString();
    if (language.isEmpty()) {
        language = QLocale::system().name();
        edSettings->setValue("language", language);
    }


    int exitCode = 0;
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


        // Close splash screen after main window showed.
        QSplashScreen *splashScreen = app.splashScreen();
        splashScreen->finish(mainWindow);
        
        
        
        
        
        auto edInterface = new SAKInterface(&app);
        auto edCrc = new SAKCrcInterface(&app);
        auto edDataStructure = new SAKDataStructure(&app);
#if 0
    QSurfaceFormat format;
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);
#endif
        auto edI18n = EDI18N::instance();
        edI18n->setConfigurationFile(":/res/i18n/easydebug.json");
        edI18n->installTranslator(language);

        auto edFontFamilies = QFontDatabase::families();
        QString family = edSettings->value("fontFamily").toString();
        if (family.isEmpty()) {
            family = app.font().family();
            edSettings->edSetValue("fontFamily", family);
        } else {
            app.setFont(family);
        }

        QQuickStyle::setStyle("Material");
        QQmlApplicationEngine qmlAppEngine;

        const QString reason = "Uncreatable type!";
        qmlRegisterType<EDBle>("ED.EasyDebug", 1, 0, "EDBle");
        qmlRegisterType<SAKCrcInterface>("ED.EasyDebug", 1, 0, "EDCrc");
        qmlRegisterType<SAKToolBox>("ED.EasyDebug", 1, 0, "EDDevice");
        qmlRegisterType<EDSerialPort>("ED.EasyDebug", 1, 0, "EDSerialPort");
        qmlRegisterType<EDHighlighter>("ED.EasyDebug", 1, 0, "EDHighlighter");
        qmlRegisterType<EDNetworkInterface>("ED.EasyDebug", 1, 0, "EDNetworkInterface");

        qmlRegisterUncreatableType<SAKBaseTool>("ED.EasyDebug", 1, 0, "EDBaseTool", reason);
        qmlRegisterUncreatableType<SAKMaskerTool>("ED.EasyDebug", 1, 0, "EDMaskerTool", reason);
        qmlRegisterUncreatableType<SAKStorerTool>("ED.EasyDebug", 1, 0, "EDStorerTool", reason);
        qmlRegisterUncreatableType<SAKEmitterTool>("ED.EasyDebug", 1, 0, "EDEmitterTool", reason);
        qmlRegisterUncreatableType<SAKAnalyzerTool>("ED.EasyDebug", 1, 0, "EDAnalyzerTool", reason);
        qmlRegisterUncreatableType<SAKToolFactory>("ED.EasyDebug", 1, 0, "EDToolsFactory", reason);
        qmlRegisterUncreatableType<SAKPrestorerTool>("ED.EasyDebug", 1, 0, "EDPrestorerTool", reason);
        qmlRegisterUncreatableType<SAKResponserTool>("ED.EasyDebug", 1, 0, "EDResponserTool", reason);
        qmlRegisterUncreatableType<SAKUdpClientTool>("ED.EasyDebug", 1, 0, "EDUdpClientTool", reason);
        qmlRegisterUncreatableType<SAKUdpServerTool>("ED.EasyDebug", 1, 0, "EDUdpServerTool", reason);
        qmlRegisterUncreatableType<SAKTcpClientTool>("ED.EasyDebug", 1, 0, "EDTcpClientTool", reason);
        qmlRegisterUncreatableType<SAKTcpServerTool>("ED.EasyDebug", 1, 0, "EDTcpServerTool", reason);
        qmlRegisterUncreatableType<SAKBleCentralTool>("ED.EasyDebug", 1, 0, "EDBleCentralTool", reason);
        qmlRegisterUncreatableType<SAKSerialPortTool>("ED.EasyDebug", 1, 0, "EDSerialportTool", reason);
        qmlRegisterUncreatableType<SAKWebSocketServerTool>("ED.EasyDebug", 1, 0, "EDWebSocketServerTool", reason);
        qmlRegisterUncreatableType<SAKWebSocketClientTool>("ED.EasyDebug", 1, 0, "EDWebSocketClientTool", reason);
        
        qmlRegisterUncreatableType<SAKInterface>("ED.EasyDebug", 1, 0, "EDInterface", reason);
        qmlRegisterUncreatableType<SAKDataStructure>("ED.EasyDebug", 1, 0, "EDDataStructure", reason);

        qmlAppEngine.rootContext()->setContextProperty("edCrc", edCrc);
        qmlAppEngine.rootContext()->setContextProperty("edI18n", edI18n);
        qmlAppEngine.rootContext()->setContextProperty("edSettings", edSettings);
        qmlAppEngine.rootContext()->setContextProperty("edInterface", edInterface);
        qmlAppEngine.rootContext()->setContextProperty("edDataStructure", edDataStructure);
        qmlAppEngine.rootContext()->setContextProperty("edFontFamilies", edFontFamilies);
        //qmlAppEngine.load("qrc:/qml/MainWindow.qml");

        // Update ui text.
        QObject::connect(edI18n, &EDI18N::languageChanged,
                         &qmlAppEngine, &QQmlApplicationEngine::retranslate);

        // If the exit code is equal to SAK_REBOOT_CODE, the application will
        // be reboot.
        exitCode = app.exec();
#ifdef SAK_REBOOT_CODE
    }while (exitCode == SAK_REBOOT_CODE);
#else
    }while (exitCode == 1314);
#endif
    return exitCode;
}
