/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QQmlContext>
#include <QQuickStyle>

#include "sakanalyzertool.h"
#include "sakbasetool.h"
#include "sakblecentraltool.h"
#include "sakblescanner.h"
#include "sakcrcinterface.h"
#include "sakdatastructure.h"
#include "sakemittertool.h"
#include "SAKGuiApplication.h"
#include "sakhighlighter.h"
#include "sakinterface.h"
#include "sakmaskertool.h"
#include "saknetworkinterfacescanner.h"
#include "sakprestorertool.h"
#include "sakresponsertool.h"
#include "sakserialportscanner.h"
#include "sakserialporttool.h"
#include "saksettings.h"
#include "sakstorertool.h"
#include "saktablemodeltool.h"
#include "saktcpclienttool.h"
#include "saktcpservertool.h"
#include "saktoolbox.h"
#include "saktoolfactory.h"
#include "saktranslator.h"
#include "sakudpclienttool.h"
#include "sakudpservertool.h"
#include "sakwebsocketclienttool.h"
#include "sakwebsocketservertool.h"

SAKGuiApplication::SAKGuiApplication(int argc, char* argv[])
    : QGuiApplication(argc, argv) {
  QQuickStyle::setStyle("Material");

  auto sakCrc = new SAKCrcInterface(this);
  auto sakInterface = new SAKInterface(this);
  auto sakDataStructure = new SAKDataStructure(this);

  auto sakI18n = SAKTranslator::instance();
  auto sakSettings = SAKSettings::instance();

  QString language = SAKSettings::instance()->language();
  SAKTranslator::instance()->setupLanguage(language);

  const QString reason = "Uncreatable type!";
  qmlRegisterType<SAKToolBox>("SAK.Custom", 1, 0, "SAKDevice");
  qmlRegisterType<SAKSettings>("SAK.Custom", 1, 0, "SAKSettings");
  qmlRegisterType<SAKBleScanner>("SAK.Custom", 1, 0, "SAKBleScanner");
  qmlRegisterType<SAKHighlighter>("SAK.Custom", 1, 0, "SAKHighlighter");
  qmlRegisterType<SAKCrcInterface>("SAK.Custom", 1, 0, "SAKCrcInterface");
  qmlRegisterType<SAKSerialPortScanner>("SAK.Custom", 1, 0,
                                        "SAKSerialPortScanner");
  qmlRegisterType<SAKNetworkInterfaceScanner>("SAK.Custom", 1, 0,
                                              "SAKNetworkInterfaceScanner");

  qmlRegisterUncreatableType<SAKBaseTool>("SAK.Custom", 1, 0, "SAKBaseTool",
                                          reason);
  qmlRegisterUncreatableType<SAKMaskerTool>("SAK.Custom", 1, 0, "SAKMaskerTool",
                                            reason);
  qmlRegisterUncreatableType<SAKStorerTool>("SAK.Custom", 1, 0, "SAKStorerTool",
                                            reason);
  qmlRegisterUncreatableType<SAKEmitterTool>("SAK.Custom", 1, 0,
                                             "SAKEmitterTool", reason);
  qmlRegisterUncreatableType<SAKToolFactory>("SAK.Custom", 1, 0,
                                             "SAKToolsFactory", reason);
  qmlRegisterUncreatableType<SAKAnalyzerTool>("SAK.Custom", 1, 0,
                                              "SAKAnalyzerTool", reason);
  qmlRegisterUncreatableType<SAKPrestorerTool>("SAK.Custom", 1, 0,
                                               "SAKPrestorerTool", reason);
  qmlRegisterUncreatableType<SAKResponserTool>("SAK.Custom", 1, 0,
                                               "SAKResponserTool", reason);
  qmlRegisterUncreatableType<SAKUdpClientTool>("SAK.Custom", 1, 0,
                                               "SAKUdpClientTool", reason);
  qmlRegisterUncreatableType<SAKUdpServerTool>("SAK.Custom", 1, 0,
                                               "SAKUdpServerTool", reason);
  qmlRegisterUncreatableType<SAKTcpClientTool>("SAK.Custom", 1, 0,
                                               "SAKTcpClientTool", reason);
  qmlRegisterUncreatableType<SAKTcpServerTool>("SAK.Custom", 1, 0,
                                               "SAKTcpServerTool", reason);
  qmlRegisterUncreatableType<SAKBleCentralTool>("SAK.Custom", 1, 0,
                                                "SAKBleCentralTool", reason);
  qmlRegisterUncreatableType<SAKSerialPortTool>("SAK.Custom", 1, 0,
                                                "SAKSerialportTool", reason);
  qmlRegisterUncreatableType<SAKTableModelTool>("SAK.Custom", 1, 0,
                                                "SAKTabelModelTool", reason);
  qmlRegisterUncreatableType<SAKCommunicationTool>(
      "SAK.Custom", 1, 0, "SAKCommunicationTool", reason);
  qmlRegisterUncreatableType<SAKWebSocketServerTool>(
      "SAK.Custom", 1, 0, "SAKWebSocketServerTool", reason);
  qmlRegisterUncreatableType<SAKWebSocketClientTool>(
      "SAK.Custom", 1, 0, "SAKWebSocketClientTool", reason);

  qmlRegisterUncreatableType<SAKInterface>("SAK.Custom", 1, 0, "SAKInterface",
                                           reason);
  qmlRegisterUncreatableType<SAKDataStructure>("SAK.Custom", 1, 0,
                                               "SAKDataStructure", reason);

  mQmlAppEngine.rootContext()->setContextProperty("sakCrc", sakCrc);
  mQmlAppEngine.rootContext()->setContextProperty("sakI18n", sakI18n);
  mQmlAppEngine.rootContext()->setContextProperty("sakSettings", sakSettings);
  mQmlAppEngine.rootContext()->setContextProperty("sakInterface", sakInterface);
  mQmlAppEngine.rootContext()->setContextProperty("sakDataStructure",
                                                  sakDataStructure);
  mQmlAppEngine.load("qrc:/qml/MainWindow.qml");
}

QQmlApplicationEngine& SAKGuiApplication::qmlAppEngine() {
  return mQmlAppEngine;
}
