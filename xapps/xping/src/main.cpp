/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "settings.h"
#include "tablemodel.h"
#include "xping.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    // xPing::setOrganizationName("xPing");
    // xPing::setApplicationName("xPing");
    // xPing::installLog(argv[0]);
    // xPing::setupHdpi();
    xPing app(argc, argv);
    // app.setupLanguage();
    // app.showSplashScreenMessage(QObject::tr("Application is booting..."));

    QQmlApplicationEngine engine;

    Settings *xSettings = new Settings();
    // QSplashScreen *xSplash = app.splashScreen();
    qmlRegisterUncreatableType<TableModel>("xPing", 1, 0, "XTableModel", "Uncreatable type");
    engine.rootContext()->setContextProperty("xApp", &app);
    engine.rootContext()->setContextProperty("xSettings", xSettings);

    // QObject::connect(&g_xTools, &xTools::xTools::languageChanged, &engine, [&engine]() {
    //     engine.retranslate();
    // });
    // QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [=]() {
    //     xSplash->close();
    // });
    // QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, [=]() {
    //     xSplash->close();
    // });
    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));

    int ret = app.exec();
    //app.uninstallLog();
    return ret;
}
