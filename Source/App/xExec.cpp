/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "App/xExec.h"

#include <QMessageBox>
#include <QProcess>

#include <glog/logging.h>

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include "Common/xToolsDataStructure.h"
#endif

void xToolsInitGoogleLogging(char *argv0)
{
    QString logPath = xTools::Settings::instance()->settingsPath();
    logPath += "/log";
    QDir dir(xTools::Settings::instance()->settingsPath());
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
    fLB::FLAGS_alsologtostderr = true;            //

    google::InitGoogleLogging(argv0);
    qInfo() << "The logging path is:" << qPrintable(logPath);
}

void xToolsShutdownGoogleLogging()
{
    google::ShutdownGoogleLogging();
}

void xToolsQtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

void xToolsInitApp(const QString &appName, bool forStore)
{
    QString cookedAppName = appName;
    if (forStore) {
        cookedAppName += QObject::tr("(Store)");
    }

    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);
    xTools::Application::setFriendlyAppName(appName);
}

void xToolsInstallMessageHandler()
{
    qInstallMessageHandler(xToolsQtLogToGoogleLog);
}

void xToolsTryToClearSettings()
{
    // Remove settings file and database
    if (!xTools::Settings::instance()->clearSettings()) {
        return;
    }

    xTools::Settings::instance()->setClearSettings(false);
    if (QFile::remove(xTools::Settings::instance()->fileName())) {
        qInfo() << "The settings file is removed.";
    } else {
        qWarning() << "The operation(remove settings file) failed!";
    }
}

void xToolsInitHdpi()
{
#if 0
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = xTools::Settings::instance()->hdpiPolicy();
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

void xToolsInitAppStyle()
{
    const QStringList keys = QStyleFactory::keys();
    qInfo() << "The supported application styles are:" << qPrintable(keys.join(QChar(',')));
    const QString style = xTools::Settings::instance()->appStyle();
    qInfo() << "The current style of application is:" << qPrintable(style);
    if (style.isEmpty()) {
        qWarning() << "The application style is not specified, the default style is:"
                   << qPrintable(QApplication::style()->objectName());
    } else if (keys.contains(style) || keys.contains(style.toLower())) {
        qInfo() << "The current style of application is:" << qPrintable(style);
        QApplication::setStyle(QStyleFactory::create(style));
    }
}

void xToolsDoSomethingBeforeAppCreated(char *argv[], const QString &appName, bool forStore)
{
    xToolsInitApp(appName, forStore);
    xToolsInitGoogleLogging(argv[0]);
#ifndef QT_DEBUG
    xToolsInstallMessageHandler();
#endif

    xToolsTryToClearSettings();
    xToolsInitHdpi();
}

void xToolsDoSomethingAfterAppExited()
{
    xToolsShutdownGoogleLogging();
}

void xToolsTryToRebootApp()
{
    int ret = QMessageBox::information(
        nullptr,
        QObject::tr("Neet to Reboot"),
        QObject::tr("The operation need to reboot to effectived, reboot the applicaion now?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath());
        qApp->closeAllWindows();
    }
}
