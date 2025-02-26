/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"

#include <QStandardPaths>

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
{
#ifdef X_TOOLS_LATEST_GIT_TAG
    QString tmp(X_TOOLS_LATEST_GIT_TAG);
    if (tmp.contains("v")) {
        tmp.remove("v");
    }
    Application::setApplicationVersion(tmp);
#else
    Application::setApplicationVersion("0.0.0");
#endif
}

Application::~Application() {}

void Application::installLog()
{
    // QString logPath = g_xTools.settingsPath();
    // logPath += "/log";
    // QDir dir(logPath);
    // if (!dir.exists(logPath) && !dir.mkpath(logPath)) {
    //     qWarning() << "Make log directory failed";
    // }

    // auto keep = std::chrono::minutes(30 * 24 * 60);
    // google::SetLogFilenameExtension(".log");     // The suffix of log file.
    // google::EnableLogCleaner(keep);              // Keep the log file for 30 days.
    // google::SetApplicationFingerprint("xTools"); // (It seem to be no use.)

    // fLB::FLAGS_logtostdout = false;
    // fLB::FLAGS_logtostderr = false;
    // fLS::FLAGS_log_dir = logPath.toUtf8().data(); // The path of log.
    // fLI::FLAGS_logbufsecs = 0;                    //
    // fLU::FLAGS_max_log_size = 10;                 // The max size(MB) of log file.
    // fLB::FLAGS_stop_logging_if_full_disk = true;  //
    // fLB::FLAGS_alsologtostderr = true;            //

    // google::InitGoogleLogging(argv0);
    // qInfo() << "The logging path is:" << qPrintable(logPath);
}

void Application::uninstallLog() {}

void Application::setupHdpi() {}

QSettings *Application::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2_v8.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}
