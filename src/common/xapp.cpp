/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xapp.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <QWidget>
#endif

#if !defined(X_DISABLE_LOG)
#include <glog/logging.h>
#endif

#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QLocale>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTimer>
#include <QTranslator>

#include "utilities/hdpimanager.h"
#include "utilities/i18n.h"
#include "utilities/stylemanager.h"
#include "utilities/thememanager.h"

class NativeEventFilter : public QAbstractNativeEventFilter
{
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override
#else
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override
#endif
    {
#ifdef Q_OS_WIN
        MSG *msg = (MSG *) message;
        if (msg->message == WM_GETOBJECT) {
            // QTBUG-77974: https://blog.csdn.net/omg_orange/article/details/116779492
            return true;
        }

        if (eventType == "windows_generic_MSG") {
            MSG *msg = static_cast<MSG *>(message);
            if (msg->message == WM_ACTIVATE) {
                HWND hwnd = msg->hwnd;
                QWidget *w = QWidget::find((WId) hwnd);
                if (w) {
                    xThemeMgr.updateWindowCaptionColor(w);
                }
            }
        }
#endif

        return false;
    }
};

xApp::xApp(int &argc, char **argv)
    : QApplication(argc, argv)
{
    xI18n.setupSettings(settings());
    xStyleMgr.setupSettings(settings());
    xThemeMgr.setupSettings(settings());
    xHdpiMgr.setupSettings(settings());

#ifdef X_LATEST_GIT_TAG
    QString tmp(X_LATEST_GIT_TAG);
    if (tmp.contains("v")) {
        tmp.remove("v");
    }
    QApplication::setApplicationVersion(tmp);
#else
    QApplication::setApplicationVersion("0.0.0");
#endif

    const ParameterKeys keys;
    bool clearSettings = xAPP->settings()->value(keys.clearSettings, false).toBool();
    if (clearSettings) {
        xAPP->settings()->clear();
        xAPP->settings()->setValue(keys.clearSettings, false);

        QString path = xAPP->settingsPath();
        QString dataJson = path + "/data.json";
        if (QFile::remove(dataJson)) {
            qInfo() << "The data.json is removed.";
        }
    }

#ifdef Q_OS_WIN
    installNativeEventFilter(new NativeEventFilter());
#endif
}

xApp::~xApp() {}

void googleLogToQtLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#if !defined(X_DISABLE_LOG)
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

void xApp::installLog(char *argv0)
{
#if !defined(QT_RELEASE)
    // Google log is just for release edition.
    return;
#endif
#if !defined(X_DISABLE_LOG)
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

void xApp::uninstallLog()
{
#if !defined(X_DISABLE_LOG)
#if defined(QT_RELEASE)
    google::ShutdownGoogleLogging();
#endif
#endif
}

void xApp::setupHdpi()
{
    xHdpiMgr.setupSettings(xApp::settings());
    xHdpiMgr.setupHdpi();
}

QSettings *xApp::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString xApp::settingsPath()
{
    QSettings *settings = xApp::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

void xApp::setupAppStyle()
{
    xStyleMgr.setupSettings(settings());
    xStyleMgr.setupAppStyle();
}

void xApp::setupLanguage()
{
    xI18n.setupSettings(settings());
    xI18n.setupLanguage();
}

void xApp::setupColorScheme()
{
    xThemeMgr.setupSettings(settings());
    xThemeMgr.updateApplicationColor();
}

void xApp::execMs(int ms)
{
    QTimer tmp;
    QEventLoop loop;
    tmp.setSingleShot(true);
    QObject::connect(&tmp, &QTimer::timeout, &loop, &QEventLoop::quit);
    tmp.start(ms);
    loop.exec();
}

QSplashScreen *xApp::splashScreen()
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

void xApp::showSplashScreenMessage(const QString &msg)
{
    QSplashScreen *splashScreen = xApp::splashScreen();
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
