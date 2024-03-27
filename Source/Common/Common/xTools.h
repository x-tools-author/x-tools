/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStyleFactory>

#ifdef X_TOOLS_USING_GLOG
#include "glog/logging.h"
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include "xToolsInterface.h"
#endif
#include "xToolsSettings.h"

#ifdef X_TOOLS_USING_GLOG

static void xToolsInitGoogleLogging(char *argv0)
{
    QString logPath = xToolsSettings::instance()->settingsPath();
    logPath += "/log";
    QDir dir(xToolsSettings::instance()->settingsPath());
    if (!dir.exists(logPath) && !dir.mkpath(logPath)) {
        qWarning() << "Make log directory failed";
    }

    google::SetLogFilenameExtension(".log");  // The suffix of log file.
    google::EnableLogCleaner(30);             // Keep the log file for 30 days.
    google::SetApplicationFingerprint("SAK"); // (It seem to be no use.)

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

static void xToolsShutdownGoogleLogging()
{
#ifndef QT_DEBUG
    google::ShutdownGoogleLogging();
#endif
}

#endif

#ifdef X_TOOLS_USING_GLOG
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
    // Initialize some information about application.
    QString cookedAppName = appName;
    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);

#ifdef X_TOOLS_VERSION
    QCoreApplication::setApplicationVersion(X_TOOLS_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif
}

static void xToolsInstallMessageHandler()
{
#ifdef X_TOOLS_USING_GLOG
    qInstallMessageHandler(qtLogToGoogleLog);
#endif
}

static void xToolsTryToClearSettings()
{
    // Remove settings file and database
    if (xToolsSettings::instance()->clearSettings()) {
        xToolsSettings::instance()->setClearSettings(false);
        if (QFile::remove(xToolsSettings::instance()->fileName())) {
            qInfo() << "The settings file is removed.";
        } else {
            qWarning() << "The operation(remove settings file) failed!";
        }
    }
}

static void xToolsInitHdpi()
{
#if 0
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = xToolsSettings::instance()->hdpiPolicy();
    if (xToolsInterface::isQtHighDpiScalePolicy(policy)) {
        const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif
}

static void xToolsInitAppStyle()
{
    const QStringList keys = QStyleFactory::keys();
    qInfo() << "The supported application styles are:" << qPrintable(keys.join(QChar(',')));
    const QString style = xToolsSettings::instance()->appStyle();
    qInfo() << style;
    if (keys.contains(style)) {
        qInfo() << "The current style of application is:" << qPrintable(style);
        QApplication::setStyle(QStyleFactory::create(style));
    }
}

static void sakDoSomethingBeforeAppCreated(char *argv[], const QString &appName)
{
    xToolsInitApp(appName);
#ifdef X_TOOLS_USING_GLOG
    xToolsInitGoogleLogging(argv[0]);
    xToolsInstallMessageHandler();
#else
    Q_UNUSED(argv)
#endif
    xToolsTryToClearSettings();
    xToolsInitHdpi();
    xToolsInitAppStyle();
}

static void sakDoSomethingAfterAppExited()
{
#ifdef X_TOOLS_USING_GLOG
    xToolsShutdownGoogleLogging();
#endif
}
