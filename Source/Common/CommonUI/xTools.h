/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <functional>
#include <type_traits>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStyle>

#include "xToolsApplication.h"
#include "xToolsMainWindow.h"
#include "xToolsSettings.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include "xToolsDataStructure.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "xToolsStyleSheetManager.h"
#endif

void xToolsInitGoogleLogging(char *argv0);
void xToolsShutdownGoogleLogging();
void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

static void xToolsInitApp(const QString &appName, bool forStore)
{
    QString cookedAppName = appName;
    if (forStore) {
        cookedAppName += QObject::tr("(Store)");
    }

    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);
    xToolsApplication::setFriendlyAppName(appName);
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

static void xToolsDoSomethingBeforeAppCreated(char *argv[],
                                              const QString &appName,
                                              bool forStore = false)
{
    xToolsInitApp(appName, forStore);
    xToolsInitGoogleLogging(argv[0]);
    xToolsInstallMessageHandler();

    xToolsTryToClearSettings();
    xToolsInitHdpi();
}

static void xToolsDoSomethingAfterAppExited()
{
    xToolsShutdownGoogleLogging();
}

template<typename CentralWidgetT = QWidget,
         typename MainWindowT = xToolsMainWindow,
         typename AppT = xToolsApplication>
int xToolsExec(int argc,
               char *argv[],
               const QString &appName,
               const QString &version = QString("0.0.0"),
               const std::function<void(void *, void *)> &doSomethingBeforAppExec = nullptr)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif

    QCoreApplication::setApplicationVersion(version);
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

    if (doSomethingBeforAppExec) {
        doSomethingBeforAppExec(ui, &app);
    }
    const int ret = app.exec();
    xToolsDoSomethingAfterAppExited();
    return ret;
}
