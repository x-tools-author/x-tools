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
#include <QStyleHints>

#include "./Application.h"
#include "./MainWindow.h"
#include "./Settings.h"

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "StyleSheetManager.h"
#endif

static void xInitResources()
{
    Q_INIT_RESOURCE(xTools);
}

namespace xTools {

void (*gOutputLog2Ui)(QtMsgType, const QMessageLogContext &, const QString &){nullptr};

void initGoogleLogging(char *argv0);
void shutdownGoogleLogging();
void qtLogToGoogleLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void initApp(const QString &appName, bool forStore);
void installMessageHandler();
void tryToClearSettings();
void initHdpi();
void initAppStyle();
void doSomethingBeforeAppCreated(char *argv[], const QString &appName, bool forStore = false);
void doSomethingAfterAppExited();
void tryToRebootApp();

/**
 * \brief exec: Using this function to start the application with a main window, you can specified
 *  the central widget type of main window. If MainWindow is the same as CentralWidget, the
 *  main window will not be created and the instance of CentralWidget will be the main window.
 * \param argc: The number of command line arguments.
 * \param argv: The command line arguments.
 * \param appName: The name of the application.
 * \param version: The version of the application.
 * \param doSomethingBeforAppExec: The function will be called before the application exec. The
 * parameter of the function is the instance of the main window and the instance of the application.
 * \return
 */
template<typename CentralWidgetT, typename MainWindowT, typename ApplicationT>
int exec(int argc,
         char *argv[],
         const QString &appName,
         const QString &version = QString("0.0.0"),
         const std::function<void(void *, void *)> &doSomethingBeforAppExec = nullptr)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif
    QCoreApplication::setApplicationVersion(version);
    doSomethingBeforeAppCreated(argv, appName);

    ApplicationT app(argc, argv);
    app.setupLanguage();

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStyleHints *styleHints = QApplication::styleHints();
    styleHints->setColorScheme(Qt::ColorScheme::Dark);
#endif

    const QString dtStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    Settings::instance()->setValue("startUpTime", dtStr);

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    auto &styleSheetManager = StyleSheetManager::singleton();
    const QString styleSheet = styleSheetManager.styleSheet();
    if (!styleSheet.isEmpty() && !styleSheetManager.currentTheme().isEmpty()) {
        app.setStyleSheet(styleSheet);
    }
#endif

    initAppStyle();

    QWidget *ui;
    if (std::is_same<CentralWidgetT, MainWindowT>::value) {
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
    Application::moveToScreenCenter(ui);
    qInfo() << "The size of window is" << ui->size();

    if (doSomethingBeforAppExec) {
        doSomethingBeforAppExec(ui, &app);
    }

    const int ret = app.exec();
    doSomethingAfterAppExited();
    return ret;
}

template<typename CentralWidget>
int execCentralWidget(int argc,
                      char *argv[],
                      const QString &appName,
                      const QString &version = QString("0.0.0"),
                      const std::function<void(void *, void *)> &doSomethingBeforAppExec = nullptr)
{
    xInitResources();
    return xTools::exec<CentralWidget, xTools::MainWindow, xTools::Application>(
        argc, argv, appName, version, doSomethingBeforAppExec);
}

class xTools : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedLanguages READ supportedLanguages CONSTANT FINAL)
private:
    explicit xTools(QObject *parent = nullptr);
    xTools(const xTools &) = delete;
    xTools &operator=(const xTools &) = delete;

public:
    ~xTools() override = default;

    static bool enableSplashScreen();
    static void setEnableSplashScreen(bool enable);
    Q_INVOKABLE void showSplashScreenMessage(const QString &msg);
    QSplashScreen &splashScreen();

    static QString friendlyAppName();
    static void setFriendlyAppName(const QString &name);
    Q_INVOKABLE QString version();
    Q_INVOKABLE QString lastCommit();
    Q_INVOKABLE QString lastCommitTime();

    Q_INVOKABLE QString defaultLanguage();
    Q_INVOKABLE void setupLanguage(const QString &language = QString());
    Q_INVOKABLE QStringList supportedLanguages();

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    Q_INVOKABLE static QVariantList supportedHighDpiPolicies();
    Q_INVOKABLE static QString highDpiPolicyName(int policy);
    Q_INVOKABLE static bool isValidHighDpiPolicy(int policy);
#endif

    // Such as "xToolsCore", "xToolsApp", "xToolsPrivateApp"
    Q_INVOKABLE virtual QStringList supportedLanguagePrefixes();
    Q_INVOKABLE void openUrl(const QString &url);

public:
    static QIcon cookedIcon(const QIcon &icon);
    static QMainWindow *mainWindow();
    static void moveToScreenCenter(QWidget *widget);

    Q_INVOKABLE static QString clipboardText();
    Q_INVOKABLE static void setClipboardText(const QString &text);

    Q_INVOKABLE static QString stringToHexString(const QString &str);
    Q_INVOKABLE static QString hexStringToString(const QString &str);

    Q_INVOKABLE static QString dateTimeString(const QString &format);
    Q_INVOKABLE static QDateTime buildDateTime();
    Q_INVOKABLE static QString buildDateTimeString(const QString &format);
    Q_INVOKABLE static QString systemDateFormat();
    Q_INVOKABLE static QString systemTimeFormat();
    Q_INVOKABLE static QString desktopPath();
    Q_INVOKABLE static QString appVersioin();
    Q_INVOKABLE static bool tryToReboot();
    Q_INVOKABLE static QByteArray byteArray2Hex(const QByteArray &source, char separator = '\0');

signals:
    void languageChanged();

protected:
    QSplashScreen m_splashScreen;

protected:
    void setupLanguageWithPrefix(const QString &language, const QString &prefix);

private:
    QMap<QString, QString> m_languageFlagNameMap;
    static bool m_enableSplashScreen;
    static QString m_friendlyAppName;

private:
    static QPixmap splashScreenPixMap();
};

} // namespace xTools
