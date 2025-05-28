/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>
#include <QPixmap>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSettings>

#include "device/blecentral.h"
#include "device/serialport.h"
#include "device/socket.h"
#include "device/tcpclient.h"
#include "device/tcpserver.h"
#include "device/udpclient.h"
#include "device/udpserver.h"
#include "device/websocketclient.h"
#include "device/websocketserver.h"

#include "common/crc.h"
#include "common/xtools.h"
#include "device/utilities/blescanner.h"
#include "device/utilities/ipscanner.h"
#include "device/utilities/serialportscanner.h"

#include "settings.h"
#include "xdebug.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    xDebug::setOrganizationName("xTools");
    xDebug::setApplicationName("xDebug");
    xDebug::setApplicationVersion(X_DEBUG_VERSION);
    xDebug::installLog(argv[0]);
    xDebug::setupHdpi();
    xDebug app(argc, argv);
    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));

    qmlRegisterType<CRC>("xTools.xDebug", 1, 0, "CRC");
    qmlRegisterType<Device>("xTools.xDebug", 1, 0, "Device");
    qmlRegisterType<Socket>("xTools.xDebug", 1, 0, "Socket");
    qmlRegisterType<UdpClient>("xTools.xDebug", 1, 0, "UdpClient");
    qmlRegisterType<UdpServer>("xTools.xDebug", 1, 0, "UdpServer");
    qmlRegisterType<TcpClient>("xTools.xDebug", 1, 0, "TcpClient");
    qmlRegisterType<TcpServer>("xTools.xDebug", 1, 0, "TcpServer");
    qmlRegisterType<IpScanner>("xTools.xDebug", 1, 0, "IpScanner");
    qmlRegisterType<BleScanner>("xTools.xDebug", 1, 0, "BleScanner");
    qmlRegisterType<BleCentral>("xTools.xDebug", 1, 0, "BleCentral");
    qmlRegisterType<SerialPort>("xTools.xDebug", 1, 0, "Serialport");
    qmlRegisterType<WebSocketServer>("xTools.xDebug", 1, 0, "WebSocketServer");
    qmlRegisterType<WebSocketClient>("xTools.xDebug", 1, 0, "WebSocketClient");
    qmlRegisterType<SerialPortScanner>("xTools.xDebug", 1, 0, "SerialPortScanner");

    CRC *xCrc = new CRC();
    Settings *xSettings = new Settings();
    QSplashScreen *xSplash = app.splashScreen();
    QQmlApplicationEngine qmlAppEngine;
#ifdef X_MAGIC
    qmlAppEngine.rootContext()->setContextProperty("xMagic", 0.3);
#else
    qmlAppEngine.rootContext()->setContextProperty("xMagic", 1.0);
#endif
    qmlAppEngine.rootContext()->setContextProperty("xApp", &app);
    qmlAppEngine.rootContext()->setContextProperty("xCrc", xCrc);
    qmlAppEngine.rootContext()->setContextProperty("xSettings", xSettings);
    qmlAppEngine.rootContext()->setContextProperty("xDebugCommit", X_GIT_COMMIT);
    qmlAppEngine.rootContext()->setContextProperty("xDebugCommitTime", X_GIT_COMMIT_TIME);
    qmlAppEngine.rootContext()->setContextProperty("xDebugVersion", X_DEBUG_VERSION);

    // clang-format off
    qmlAppEngine.rootContext()->setContextProperty("xBin", QVariant(static_cast<int>(TextFormat::Bin)));
    qmlAppEngine.rootContext()->setContextProperty("xOct", QVariant(static_cast<int>(TextFormat::Oct)));
    qmlAppEngine.rootContext()->setContextProperty("xDec", QVariant(static_cast<int>(TextFormat::Dec)));
    qmlAppEngine.rootContext()->setContextProperty("xHex", QVariant(static_cast<int>(TextFormat::Hex)));
    qmlAppEngine.rootContext()->setContextProperty("xAscii", QVariant(static_cast<int>(TextFormat::Ascii)));
    qmlAppEngine.rootContext()->setContextProperty("xUtf8", QVariant(static_cast<int>(TextFormat::Utf8)));

    qmlAppEngine.rootContext()->setContextProperty("xEscNone", QVariant(static_cast<int>(EscapeCharacter::None)));
    qmlAppEngine.rootContext()->setContextProperty("xEscN", QVariant(static_cast<int>(EscapeCharacter::N)));
    qmlAppEngine.rootContext()->setContextProperty("xEscR", QVariant(static_cast<int>(EscapeCharacter::R)));
    qmlAppEngine.rootContext()->setContextProperty("xEscRN", QVariant(static_cast<int>(EscapeCharacter::RN)));
    qmlAppEngine.rootContext()->setContextProperty("xEscNR", QVariant(static_cast<int>(EscapeCharacter::NR)));
    qmlAppEngine.rootContext()->setContextProperty("xEscR_N", QVariant(static_cast<int>(EscapeCharacter::R_N)));

    qmlAppEngine.rootContext()->setContextProperty("xAffixesNone", QVariant(static_cast<int>(Affixes::None)));
    qmlAppEngine.rootContext()->setContextProperty("xAffixesN", QVariant(static_cast<int>(Affixes::N)));
    qmlAppEngine.rootContext()->setContextProperty("xAffixesR", QVariant(static_cast<int>(Affixes::R)));
    qmlAppEngine.rootContext()->setContextProperty("xAffixesRN", QVariant(static_cast<int>(Affixes::RN)));
    qmlAppEngine.rootContext()->setContextProperty("xAffixesNR", QVariant(static_cast<int>(Affixes::NR)));
    // clang-format on

    const QUrl qml = QStringLiteral("qrc:/qml/MainWindow.qml");
    QObject::connect(&app, &xDebug::languageChanged, &qmlAppEngine, [&qmlAppEngine]() {
        qmlAppEngine.retranslate();
    });
    QObject::connect(
        &qmlAppEngine,
        &QQmlApplicationEngine::objectCreationFailed,
        xApp,
        [xSplash]() { xSplash->hide(); },
        Qt::QueuedConnection);
    QObject::connect(
        &qmlAppEngine,
        &QQmlApplicationEngine::objectCreated,
        xApp,
        [xSplash, qml](QObject *, const QUrl &url) {
            if (qml == url) {
                xSplash->hide();
            }
        },
        Qt::QueuedConnection);

    qmlAppEngine.load(qml);
    int ret = app.exec();
    app.uninstallLog();
    return ret;
}
