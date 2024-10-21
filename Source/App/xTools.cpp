/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "App/xTools.h"

#include <QMessageBox>
#include <QProcess>

#include <glog/logging.h>

namespace xTools {

static void failureWriter(const char *data, size_t size)
{
#if 0
    QByteArray localMsg(data, size);
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString logName = QString("crash_%1.log").arg(currentDateTime);
    QFile file(Settings::instance()->settingsPath() + QString("/log/") + logName);
    QDataStream out(&file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        out << localMsg;
        file.close();
    }
#else
    Q_UNUSED(data);
    Q_UNUSED(size);
    auto ret = QMessageBox::warning(
        nullptr,
        QObject::tr("Critical Error"),
        QObject::tr("The application has been crashed, clear settings file(all settings data of "
                    "the application will be clear!) and reboot the application?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        tryToClearSettings();
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
    }
#endif
}

void initGoogleLogging(char *argv0)
{
    QString logPath = Settings::instance()->settingsPath();
    logPath += "/log";
    QDir dir(Settings::instance()->settingsPath());
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

    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(failureWriter);

    google::InitGoogleLogging(argv0);
    qInfo() << "The logging path is:" << qPrintable(logPath);
}

void shutdownGoogleLogging()
{
    google::ShutdownGoogleLogging();
}

void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

void initApp(const QString &appName, bool forStore)
{
    QString cookedAppName = appName;
    if (forStore) {
        cookedAppName += QObject::tr("(Store)");
    }

    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);
    Application::setFriendlyAppName(appName);
}

void installMessageHandler()
{
    qInstallMessageHandler(qtLogToGoogleLog);
}

void tryToClearSettings()
{
    // Remove settings file and database
    if (!Settings::instance()->clearSettings()) {
        return;
    }

    Settings::instance()->setClearSettings(false);
    if (QFile::remove(Settings::instance()->fileName())) {
        qInfo() << "The settings file is removed.";
    } else {
        qWarning() << "The operation(remove settings file) failed!";
    }
}

void initHdpi()
{
#if 0
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = Settings::instance()->hdpiPolicy();
    if (!Application::isValidHighDpiPolicy(policy)) {
        qWarning() << "The value of hdpi policy is not specified, set to default value:"
                   << QGuiApplication::highDpiScaleFactorRoundingPolicy();
        return;
    }

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
#endif
}

void initAppStyle()
{
    const QStringList keys = QStyleFactory::keys();
    qInfo() << "The supported application styles are:" << qPrintable(keys.join(QChar(',')));
    const QString style = Settings::instance()->appStyle();
    if (style.isEmpty()) {
        qWarning() << "The application style is not specified, the default style is:"
                   << qPrintable(QApplication::style()->objectName());
    } else if (keys.contains(style) || keys.contains(style.toLower())) {
        qInfo() << "The current style of application is:" << qPrintable(style);
        QApplication::setStyle(QStyleFactory::create(style));
    }
}

void doSomethingBeforeAppCreated(char *argv[], const QString &appName, bool forStore)
{
    initApp(appName, forStore);
    initGoogleLogging(argv[0]);
#ifdef QT_RELEASE
    installMessageHandler();
#endif

    tryToClearSettings();
    initHdpi();
}

void doSomethingAfterAppExited()
{
    shutdownGoogleLogging();
}

void tryToRebootApp()
{
    int ret = QMessageBox::information(
        nullptr,
        QObject::tr("Neet to Reboot"),
        QObject::tr("The operation need to reboot to effectived, reboot the applicaion now?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        qApp->closeAllWindows();
    }
}

} // namespace xTools
