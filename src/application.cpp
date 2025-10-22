/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"

#if !defined(X_DISABLE_GLOG)
#include <glog/logging.h>
#endif

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFileInfoList>
#include <QLocale>
#include <QPainter>
#include <QScreen>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTimer>
#include <QTranslator>

#include "common/xtools.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
#ifdef X_LATEST_GIT_TAG
    QString tmp(X_LATEST_GIT_TAG);
    if (tmp.contains("v")) {
        tmp.remove("v");
    }
    Application::setApplicationVersion(tmp);
#else
    Application::setApplicationVersion("0.0.0");
#endif

    bool clearSettings = Application::settings()->value(SettingsKey().clearSettings, false).toBool();
    if (clearSettings) {
        Application::settings()->clear();
        Application::settings()->setValue(SettingsKey().clearSettings, false);

        QString path = Application::settingsPath();
        QString dataJson = path + "/data.json";
        if (QFile::remove(dataJson)) {
            qInfo() << "The data.json is removed.";
        }
    }
}

void googleLogToQtLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#if !defined(X_DISABLE_GLOG)
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
#else
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);
#endif
}

void Application::installLog(char *argv0)
{
#if !defined(QT_RELEASE)
    // Google log is just for release edition.
    return;
#endif
#if !defined(X_DISABLE_GLOG)
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
#else
    Q_UNUSED(argv0);
#endif
}

void Application::uninstallLog()
{
#if !defined(X_DISABLE_GLOG)
#if defined(QT_RELEASE)
    google::ShutdownGoogleLogging();
#endif
#endif
}

void Application::setupHdpi()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QSettings *settings = Application::settings();
    int defaultPolicy = static_cast<int>(QGuiApplication::highDpiScaleFactorRoundingPolicy());
    int policy = settings->value(SettingsKey().hdpi, defaultPolicy).toInt();

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
#endif
}

QSettings *Application::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString Application::settingsPath()
{
    QSettings *settings = Application::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

void Application::setupAppStyle()
{
    QSettings *settings = Application::settings();
    if (!qApp || !Application::style()) {
        return;
    }

    QString defaultStyle = xDefaultStyleName;
    QString style = settings->value(SettingsKey().style, defaultStyle).toString();
    for (QString &key : QStyleFactory::keys()) {
        if (key.toLower() == style.toLower()) {
            Application::setStyle(QStyleFactory::create(style));
        }
    }

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

void setupLanguage(const QString &qmFile)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString tmp = qmFile;
    tmp = tmp.remove(appPath + "/");

    QTranslator *translator = new QTranslator();
    if (!translator->load(qmFile)) {
        qWarning() << QString("The qm file(%1) can not be loaded.").arg(tmp);
        return;
    }

    if (!qApp->installTranslator(translator)) {
        qWarning() << QString("The qm file(%1) can not be installed.").arg(tmp);
    }

    qInfo() << QString("The qm file(%1) is installed.").arg(tmp);
}

void findQmFile(const QString &path, const QString &language, QStringList &qmFiles)
{
    QDir dir(path);
    QFileInfoList infos = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &info : infos) {
        if (info.isDir()) {
            findQmFile(info.absoluteFilePath(), language, qmFiles);
        } else if (info.isFile() && info.suffix().toLower() == "qm") {
            QString fileName = info.fileName();
            if (fileName.endsWith("_" + language + ".qm")) {
                qmFiles.append(info.absoluteFilePath());
            }
        }
    }
}

void Application::setupLanguage()
{
    QSettings *settings = Application::settings();
    QString defaultLanguage = QLocale::system().name();
    QString language = settings->value(SettingsKey().language, defaultLanguage).toString();

    const QString appPath = QApplication::applicationDirPath();
    const QString qmFilesPath = appPath + "/translations";

    qInfo() << "The qm files path is:" << qmFilesPath;

    QStringList qmFiles;
    findQmFile(qmFilesPath, language, qmFiles);
    for (const QString &qmFile : qmFiles) {
        ::setupLanguage(qmFile);
    }
}

void Application::setupColorScheme()
{
#if xEnableColorScheme
    QSettings *settings = Application::settings();
    auto def = qApp->styleHints()->colorScheme();
    int colorScheme = settings->value(SettingsKey().colorScheme, static_cast<int>(def)).toInt();
    Qt::ColorScheme cookedColorScheme = static_cast<Qt::ColorScheme>(colorScheme);
    QStyleHints *hints = qApp->styleHints();
    hints->setColorScheme(cookedColorScheme);
    qInfo() << "The current color scheme is:" << qApp->styleHints()->colorScheme();
#endif
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
#if defined(Q_OS_ANDROID)
        QScreen *screen = QGuiApplication::primaryScreen();
        int w = 600;
        int h = 260;
        if (screen) {
            QSize size = screen->availableSize();
            w = size.width();
            h = size.height();
        }
        QPixmap pixMap(w, h);
        Q_UNUSED(fontMetrics);
#else
        int width = fontMetrics.boundingRect(displayName).width() * 1.2;
        QPixmap pixMap(width < 600 ? 600 : width, 260);
#endif
        pixMap.fill(QColor(0x1f1f1f));

        QPainter painter(&pixMap);
        painter.setPen(QColor(Qt::white));
        painter.setFont(font);
        painter.drawText(pixMap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
        painter.setPen(QColor(0x1f2c9f));
        painter.drawRect(pixMap.rect() - QMargins(1, 1, 1, 1));

        splashScreen = new QSplashScreen(pixMap);
        splashScreen->setDisabled(true);
    }

    return splashScreen;
}

void Application::showSplashScreenMessage(const QString &msg)
{
    QSplashScreen *splashScreen = Application::splashScreen();
    if (!splashScreen) {
        return;
    }

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    splashScreen->showFullScreen();
#else
#if !defined(X_MAGIC)
    splashScreen->show();
#endif
#endif
    splashScreen->showMessage(msg, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QApplication::processEvents();
}

QString Application::left2rightTips() const
{
    return tr("Transfer rx bytes with right page");
}

QString Application::right2leftTips() const
{
    return tr("Transfer rx bytes with left page");
}
