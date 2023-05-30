/******************************************************************************
 * Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QQuickStyle>
#include <QQmlContext>

#include "SAKI18N.hh"
#include "SAKSettings.hh"
#include "SAKInterface.hh"
#include "SAKBleScanner.hh"
#include "SAKHighlighter.hh"
#include "SAKCrcInterface.hh"
#include "SAKDataStructure.hh"
#include "SAKSerialPortScanner.hh"
#include "SAKNetworkInterfaceScanner.hh"

#include "SAKToolBox.hh"
#include "SAKBaseTool.hh"
#include "SAKMaskerTool.hh"
#include "SAKStorerTool.hh"
#include "SAKEmitterTool.hh"
#include "SAKAnalyzerTool.hh"
#include "SAKToolFactory.hh"
#include "SAKUdpClientTool.hh"
#include "SAKUdpServerTool.hh"
#include "SAKTcpClientTool.hh"
#include "SAKTcpServerTool.hh"
#include "SAKPrestorerTool.hh"
#include "SAKResponserTool.hh"
#include "SAKSerialPortTool.hh"
#include "SAKBleCentralTool.hh"
#include "SAKWebSocketClientTool.hh"
#include "SAKWebSocketServerTool.hh"

#include "SAKGuiApplication.hh"

SAKGuiApplication::SAKGuiApplication(int argc, char *argv[])
    : QGuiApplication(argc, argv)
{
    QQuickStyle::setStyle("Material");

    auto sakCrc = new SAKCrcInterface(this);
    auto sakInterface = new SAKInterface(this);
    auto sakDataStructure = new SAKDataStructure(this);

    auto sakI18n = SAKI18N::instance();
    auto sakSettings = SAKSettings::instance();

    const QString reason = "Uncreatable type!";
    qmlRegisterType<SAKBleScanner>("ED.EasyDebug", 1, 0, "EDBle");
    qmlRegisterType<SAKCrcInterface>("ED.EasyDebug", 1, 0, "EDCrc");
    qmlRegisterType<SAKToolBox>("ED.EasyDebug", 1, 0, "EDDevice");
    qmlRegisterType<SAKSerialPortScanner>("ED.EasyDebug", 1, 0, "EDSerialPort");
    qmlRegisterType<SAKHighlighter>("ED.EasyDebug", 1, 0, "EDHighlighter");
    qmlRegisterType<SAKNetworkInterfaceScanner>("ED.EasyDebug", 1, 0, "EDNetworkInterface");

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
    qmlRegisterUncreatableType<SAKWebSocketServerTool>("ED.EasyDebug", 1, 0, "SAKWebSocketServerTool", reason);
    qmlRegisterUncreatableType<SAKWebSocketClientTool>("ED.EasyDebug", 1, 0, "EDWebSocketClientTool", reason);

    qmlRegisterUncreatableType<SAKInterface>("ED.EasyDebug", 1, 0, "EDInterface", reason);
    qmlRegisterUncreatableType<SAKDataStructure>("ED.EasyDebug", 1, 0, "EDDataStructure", reason);

    mQmlAppEngine.rootContext()->setContextProperty("sakCrc", sakCrc);
    mQmlAppEngine.rootContext()->setContextProperty("sakI18n", sakI18n);
    mQmlAppEngine.rootContext()->setContextProperty("sakSettings", sakSettings);
    mQmlAppEngine.rootContext()->setContextProperty("sakInterface", sakInterface);
    mQmlAppEngine.rootContext()->setContextProperty("sakDataStructure", sakDataStructure);
    mQmlAppEngine.load("qrc:/qml/MainWindow.qml");
}

QQmlApplicationEngine &SAKGuiApplication::qmlAppEngine()
{
    return mQmlAppEngine;
}
