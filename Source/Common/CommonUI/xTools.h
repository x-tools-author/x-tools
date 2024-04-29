/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <type_traits>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStyle>

#include "xToolsApplication.h"
#include "xToolsMainWindow.h"
#include "xToolsSettings.h"

#ifdef X_TOOLS_ENABLE_MODULE_GLOG
#include "glog/logging.h"
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include "xToolsDataStructure.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "xToolsStyleSheetManager.h"
#endif

#ifdef X_TOOLS_ENABLE_MODULE_GLOG
static void xToolsInitGoogleLogging(char *argv0)
{
    QString logPath = xToolsSettings::instance()->settingsPath();
    logPath += "/log";
    QDir dir(xToolsSettings::instance()->settingsPath());
    if (!dir.exists(logPath) && !dir.mkpath(logPath)) {
        qWarning() << "Make log directory failed";
    }

    auto keep = std::chrono::minutes(30 * 24 * 60);
    google::SetLogFilenameExtension(".log");     // The suffix of log file.
    google::EnableLogCleaner(keep);              // Keep the log file for 30 days.
    google::SetApplicationFingerprint("xTools"); // (It seem to be no use.)

    fLB::FLAGS_logtostdout = false;
    fLB::FLAGS_logtostderr = false;
    fLS::FLAGS_log_dir = logPath.toUtf8().data(); // The path of log.
    fLI::FLAGS_logbufsecs = 0;                    //
    fLU::FLAGS_max_log_size = 10;                 // The max size(MB) of log file.
    fLB::FLAGS_stop_logging_if_full_disk = true;  //
    fLB::FLAGS_alsologtostderr = false;           //

#ifndef QT_DEBUG
    google::InitGoogleLogging(argv0);
#endif
    qInfo() << "The logging path is:" << qPrintable(logPath);
}
#endif

#ifdef X_TOOLS_ENABLE_MODULE_GLOG
static void xToolsShutdownGoogleLogging()
{
#ifndef QT_DEBUG
    google::ShutdownGoogleLogging();
#endif
}
#endif

#ifdef X_TOOLS_ENABLE_MODULE_GLOG
static void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toUtf8();
    const char *file = context.file ? context.file : "";
    const int line = context.line;

    switch (type) {
    case QtWarningMsg:
        google::LogMessage(file, line, google::GLOG_WARNING).stream() << localMsg.data();
        break;
    case QtCriticalMsg:
        google::LogMessage(file, line, google::GLOG_ERROR).stream() << localMsg.data();
        break;
    case QtFatalMsg:
        google::LogMessage(file, line, google::GLOG_FATAL).stream() << localMsg.data();
        break;
    default:
        google::LogMessage(file, line, google::GLOG_INFO).stream() << localMsg.data();
        break;
    }
}
#endif

static void xToolsInitApp(const QString &appName)
{
    QString cookedAppName = appName;
#ifdef X_TOOLS_BUILD_FOR_STORE
    cookedAppName += QObject::tr("(Store)");
#endif
    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);
    QApplication::setApplicationDisplayName(appName);

#ifdef X_TOOLS_VERSION
    QCoreApplication::setApplicationVersion(X_TOOLS_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif
}

static void xToolsInstallMessageHandler()
{
#ifdef X_TOOLS_ENABLE_MODULE_GLOG
    qInstallMessageHandler(qtLogToGoogleLog);
#endif
}

static void xToolsTryToClearSettings()
{
    // Remove settings file and database
    if (!xToolsSettings::instance()->clearSettings()) {
        return;
    }

    xToolsSettings::instance()->setClearSettings(false);
    if (QFile::remove(xToolsSettings::instance()->fileName())) {
        qInfo() << "The settings file is removed.";
    } else {
        qWarning() << "The operation(remove settings file) failed!";
    }
}

static void xToolsInitHdpi()
{
#if 0
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = xToolsSettings::instance()->hdpiPolicy();
    if (!xToolsDataStructure::isValidHighDpiPolicy(policy)) {
        qWarning() << "The value of hdpi policy is not specified, set to default value:"
                   << QGuiApplication::highDpiScaleFactorRoundingPolicy();
        return;
    }

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
#endif
}

static void xToolsInitAppStyle()
{
    const QStringList keys = QStyleFactory::keys();
    qInfo() << "The supported application styles are:" << qPrintable(keys.join(QChar(',')));
    const QString style = xToolsSettings::instance()->appStyle();
    qInfo() << "The current style of application is:" << qPrintable(style);
    if (style.isEmpty()) {
        qWarning() << "The application style is not specified, the default style is:"
                   << qPrintable(QApplication::style()->objectName());
    } else if (keys.contains(style) || keys.contains(style.toLower())) {
        qInfo() << "The current style of application is:" << qPrintable(style);
        QApplication::setStyle(QStyleFactory::create(style));
    }
}

static void xToolsDoSomethingBeforeAppCreated(char *argv[], const QString &appName)
{
    xToolsInitApp(appName);
#ifdef X_TOOLS_ENABLE_MODULE_GLOG
    xToolsInitGoogleLogging(argv[0]);
    xToolsInstallMessageHandler();
#else
    Q_UNUSED(argv)
#endif
    xToolsTryToClearSettings();
    xToolsInitHdpi();
}

static void xToolsDoSomethingAfterAppExited()
{
#ifdef X_TOOLS_ENABLE_MODULE_GLOG
    xToolsShutdownGoogleLogging();
#endif
}

template<typename CentralWidgetT = QWidget,
         typename MainWindowT = xToolsMainWindow,
         typename AppT = xToolsApplication>
int xToolsExec(int argc, char *argv[], const QString &appName)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif

    xToolsDoSomethingBeforeAppCreated(argv, appName);

    AppT app(argc, argv);
    const QString dtStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    xToolsSettings::instance()->setValue("startUpTime", dtStr);

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    auto &styleSheetManager = xToolsStyleSheetManager::instance();
    const QString styleSheet = styleSheetManager.styleSheet();
    if (!styleSheet.isEmpty() && !styleSheetManager.currentTheme().isEmpty()) {
        app.setStyleSheet(styleSheet);
    }
#else
    xToolsInitAppStyle();
#endif

    QWidget *ui;
    if (std::is_same<MainWindowT, CentralWidgetT>::value) {
        auto widget = new CentralWidgetT();
        ui = widget;
    } else {
        auto mainWindow = new MainWindowT();
        auto centralWidget = new CentralWidgetT(mainWindow);
        mainWindow->setWindowTitle(appName);
        mainWindow->setCentralWidget(centralWidget);
        ui = mainWindow;
    }

    QSplashScreen &splashScreen = app.splashScreen();
    splashScreen.finish(ui);
    ui->show();
    ui->resize(static_cast<int>(static_cast<qreal>(ui->height()) * 1.732), ui->height());
    xToolsApplication::moveToScreenCenter(ui);
    qInfo() << "The size of window is" << ui->size();

    const int ret = app.exec();
    xToolsDoSomethingAfterAppExited();
    return ret;
}
