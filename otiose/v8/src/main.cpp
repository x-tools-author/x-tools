/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "application.h"
#include "tools/toos.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    Application app(argc, argv);
    app.setupLanguage();
    QQuickStyle::setStyle("Material");
    //QQuickStyle::setStyle("Fusion");
    //QQuickStyle::setStyle("FluentWinUI3");
    //QQuickStyle::setStyle("Imagine");
    //QQuickStyle::setStyle("Universal");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("xMagic", 1); // just for test
    engine.rootContext()->setContextProperty("xApp", &app);

    Tools::registerAllTools();

    static const QString reason = QStringLiteral("Cannot create instance of Application");
    qmlRegisterUncreatableType<Application>("xTools", 1, 0, "App", reason);

    QObject::connect(&app, &Application::languageChanged, &engine, &QQmlEngine::retranslate);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("xToolsQml", "Main");

    return app.exec();
}
