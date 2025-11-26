/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDebug>
#include <QMessageBox>

#if X_ENABLE_HID
#include <hidapi.h>
#endif

#include "application.h"
#include "mainwindow.h"
#include "utilities/thememanager.h"

#if X_ENABLE_SINGLE_APPLICATION
#include "singleapplication.h"
#endif

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <QAbstractNativeEventFilter>
// QTBUG-77974: https://blog.csdn.net/omg_orange/article/details/116779492
class NativeEventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override
    {
        MSG *msg = (MSG *) message;
        if (msg->message == WM_GETOBJECT) {
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

        return false;
    }
};
#endif

int main(int argc, char *argv[])
{
    Application::setOrganizationName("xTools");
    Application::setApplicationName("xTools");
    Application::installLog(argv[0]);
    Application::setupHdpi();
    Application app(argc, argv);
#ifdef Q_OS_WIN
    app.installNativeEventFilter(new NativeEventFilter());
#endif

#if X_ENABLE_SINGLE_APPLICATION
    SingleApplication sApp(argc, argv);
    if (sApp.isSecondary()) {
        return 0;
    }
#endif

    app.setupLanguage();
    app.showSplashScreenMessage(QObject::tr("Application is booting..."));
    app.setupAppStyle();
    app.setupColorScheme();

#if X_ENABLE_HID
    hid_init();
#endif

    MainWindow window;
    QSplashScreen *splash = app.splashScreen();
    splash->finish(&window);
    window.resize(1280, 720);
    window.show();
    window.moveToCenter();
    window.load();

#if X_ENABLE_SINGLE_APPLICATION
    QObject::connect(&sApp, &SingleApplication::instanceStarted, &window, [&window]() {
        window.show();
        window.raise();
        window.activateWindow();
        QApplication::beep();
    });
#endif

    int ret = app.exec();

#if X_ENABLE_HID
    hid_exit();
#endif

    Application::uninstallLog();
    qInfo() << "Application exited with code:" << ret;
    return ret;
}
