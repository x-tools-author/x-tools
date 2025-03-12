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

#include "Common/xTools.h"
#include "TableModel.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(xTools);
    Q_INIT_RESOURCE(xApp);
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    xTools::xTools::doSomethingBeforeAppCreated(argv, "xPing");
    QApplication::setApplicationVersion(QString(X_PING_VERSION));
    QApplication app(argc, argv);
    g_xTools.splashScreenShow();
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<TableModel>("xPing", 1, 0, "XTableModel", "Uncreatable type");
    engine.rootContext()->setContextProperty("xSettings", g_xTools.settings());

    QObject::connect(&g_xTools, &xTools::xTools::languageChanged, &engine, [&engine]() {
        engine.retranslate();
    });
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [&app]() {
        g_xTools.splashScreenGet()->close();
    });
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, [&app]() {
        g_xTools.splashScreenGet()->close();
    });
    engine.load(QUrl(QStringLiteral("qrc:/Qml/MainWindow.qml")));

    int ret = app.exec();
    xTools::xTools::doSomethingAfterAppExited();
    return ret;
}
