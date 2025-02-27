/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"

#include <glog/logging.h>

#include <QDir>
#include <QEventLoop>
#include <QLocale>
#include <QPainter>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>
#include <QTimer>
#include <QTranslator>

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

void googleLogToQtLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

void Application::installLog(char *argv0)
{
#if !defined(QT_RELEASE)
    // Google log is just for release edition.
    return;
#endif

    // Redirect the log message to Qt log.
    qInstallMessageHandler(googleLogToQtLog);

    // Google log initializing...
    QString logPath = settingsPath();
    logPath += "/log";
    QDir dir(logPath);
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

    google::InitGoogleLogging(argv0);
    qInfo() << "The logging path is:" << qPrintable(logPath);
}

void Application::uninstallLog()
{
#if !defined(QT_RELEASE)
    google::ShutdownGoogleLogging();
#endif
}

void Application::setupHdpi()
{
    QSettings *settings = Application::settings();
    int defaultPolicy = static_cast<int>(QGuiApplication::highDpiScaleFactorRoundingPolicy());
    int policy = settings->value(SettingsKey().hdpi, defaultPolicy).toInt();

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
}

void Application::setupAppStyle()
{
    QSettings *settings = Application::settings();
    if (!Application::style()) {
        return;
    }

    QString defaultStyle = Application::style()->name();
    QString style = settings->value(SettingsKey().style, defaultStyle).toString();
    Application::setStyle(QStyleFactory::create(style));
    qInfo() << "The current application style is:" << style;
}

void Application::execMs(int ms)
{
    QTimer tmp;
    QEventLoop loop;
    tmp.setSingleShot(true);
    QObject::connect(&tmp, &QTimer::timeout, &loop, &QEventLoop::quit);
    tmp.start(ms);
    loop.exec();
}

void Application::setupLanguage()
{
    QSettings *settings = Application::settings();
    QString defaultLanguage = QLocale::system().name();
    QString language = settings->value(SettingsKey().language, defaultLanguage).toString();

    QTranslator *translator = new QTranslator();
    if (!translator->load(QString(":/res/translations/xTools_%2.qm").arg(language))) {
        qWarning() << "The language file can not be load to translator, English will be used.";
        return;
    }

    if (!qApp->installTranslator(translator)) {
        qInfo() << "The language has been setup, English will be used.";
    } else {
        qInfo() << "The language has been setup, current language is:" << language;
    }
}

QSettings *Application::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2_v8.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString Application::settingsPath()
{
    QSettings *settings = Application::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

QSplashScreen *Application::splashScreen()
{
    if (!qApp) {
        return Q_NULLPTR;
    }

    static QSplashScreen *splashScreen = Q_NULLPTR;
    if (!splashScreen) {
        QFont font = qApp->font();
        font.setPixelSize(52);

        QFontMetrics fontMetrics(font);
        const QString displayName = applicationName();
        int width = fontMetrics.boundingRect(displayName).width() * 1.2;

        QPixmap pixMap(width < 600 ? 600 : width, 260);
        pixMap.fill(QColor(0x2d2d30));

        QPainter painter(&pixMap);
        painter.setPen(QColor(Qt::white));
        painter.setFont(font);
        painter.drawText(pixMap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
        painter.drawRect(pixMap.rect() - QMargins(1, 1, 1, 1));

        splashScreen = new QSplashScreen(pixMap);
        splashScreen->setDisabled(true);
    }

    return splashScreen;
}

void Application::showSplashScreenMessage(const QString &msg)
{
    if (!qApp) {
        return;
    }

    static QSplashScreen *splashScreen = Application::splashScreen();
    if (splashScreen) {
        splashScreen->show();
        splashScreen->showMessage(msg, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
        QApplication::processEvents();
    }
}
