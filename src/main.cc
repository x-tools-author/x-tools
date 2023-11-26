/***************************************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDir>
#include <QFile>
#include <QStyleFactory>

#ifdef SAK_USING_GLOG
#include "glog/logging.h"
#endif

#include "sakapplication.h"
#include "sakinterface.h"
#include "saklog.h"
#include "saksettings.h"

#ifdef SAK_USING_GLOG

void sakInitGoogleLogging(char *argv0)
{
    QString logPath = SAKSettings::instance()->settingsPath();
    logPath += "/log";
    QDir dir(SAKSettings::instance()->settingsPath());
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

void sakShutdownGoogleLogging()
{
#ifndef QT_DEBUG
    google::ShutdownGoogleLogging();
#endif
}

#endif

void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toUtf8();
    const char *file = context.file ? context.file : "";
    const int line = context.line;

#ifdef SAK_USING_GLOG
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
#else
    Q_UNUSED(localMsg)
    Q_UNUSED(file)
    Q_UNUSED(line)
#endif
    // Show the log on ui.
    SAKLog::instance()->appendLog(type, context, msg);
}

static void sakInitApp()
{
    // Initialize some information about application.
    QCoreApplication::setOrganizationName(QString("Qsaker"));
    QCoreApplication::setOrganizationDomain(QString("IT"));

    QString appName = QString("QtSwissArmyKnife");
#ifndef SAK_IMPORT_MODULE_PRIVATE
    appName += QObject::tr("(Community)");
#endif
    QCoreApplication::setApplicationName(appName);

#ifdef SAK_VERSION
    QCoreApplication::setApplicationVersion(SAK_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif
}

static void sakInstallMessageHandler()
{
#ifndef QT_DEBUG
    qInstallMessageHandler(qtLogToGoogleLog);
#endif
}

static void sakTryToClearSettings()
{
    // Remove settings file and database
    if (SAKSettings::instance()->clearSettings()) {
        SAKSettings::instance()->setClearSettings(false);
        if (QFile::remove(SAKSettings::instance()->fileName())) {
            qInfo() << "Remove settings file successfully.";
        } else {
            qWarning() << "Remove settings file failed!";
        }
    }
}

static void sakInitHdpi()
{
    // qputenv("QT_SCALE_FACTOR", "1.5");
    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if (int policy = SAKSettings::instance()->hdpiPolicy();
        SAKInterface::isQtHighDpiScalePolicy(policy)) {
        const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif
}

static void sakInitAppStyle()
{
    qInfo() << "Supported style:" << QStyleFactory::keys();
    if (const QString style = SAKSettings::instance()->appStyle();
        QStyleFactory::keys().contains(style)) {
        qInfo() << "App style:" << style;
        QApplication::setStyle(QStyleFactory::create(style));
    }
}

int main(const int argc, char *argv[])
{
    sakInitApp();
    sakInitGoogleLogging(argv[0]);
    sakInstallMessageHandler();
    sakTryToClearSettings();
    sakInitHdpi();
    sakInitAppStyle();

    SAKLog::instance()->start();
    SAKApplication app(argc, argv);
    int ret = SAKApplication::exec();
    sakShutdownGoogleLogging();
    return ret;
}
