/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "settings.h"
#include "tablemodel.h"
#include "xping.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    QApplication::setOrganizationName("xTools");
    QApplication::setApplicationName("xPing");
    QApplication::setApplicationVersion(X_PING_VERSION);
    xPing app(argc, argv);
    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));

    QQmlApplicationEngine engine;

    Settings *xSettings = app.settings();
    QSplashScreen *xSplash = app.splashScreen();
    qmlRegisterUncreatableType<TableModel>("xPing", 1, 0, "XTableModel", "Uncreatable type");
    engine.rootContext()->setContextProperty("xApp", &app);
    engine.rootContext()->setContextProperty("xSettings", xSettings);
#ifdef X_MAGIC
    engine.rootContext()->setContextProperty("xMagic", 0.3);
#else
    engine.rootContext()->setContextProperty("xMagic", 1.0);
#endif

    QObject::connect(&app, &xPing::languageChanged, &engine, [&engine]() { engine.retranslate(); });
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [=]() {
        xSplash->close();
    });
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, [=]() {
        xSplash->close();
    });
    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));

    return app.exec();
}
