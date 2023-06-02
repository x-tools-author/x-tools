/******************************************************************************
 * Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QScreen>
#include <QPushButton>
#include <QGridLayout>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#endif

#include <QLocale>
#include <QVariant>
#include <QFontDatabase>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "SAKI18N.hh"
#include "SAKSettings.hh"
#include "SAKApplication.hh"
#include "SAKGuiApplication.hh"

int main(int argc, char *argv[])
{
    // Initialize some information about application.
    QCoreApplication::setOrganizationName(QString("Qsaker"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(QString("QtSwissArmyKnife"));
#ifdef SAK_VERSION
    QCoreApplication::setApplicationVersion(SAK_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif

    // High dpi settings.
    auto sakSettings = SAKSettings::instance();
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
    auto policy = sakSettings->hdpiPolicy();
    auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
#endif

    // Language setttings.
    auto language = sakSettings->value("language").toString();
    if (language.isEmpty()) {
        language = QLocale::system().name();
        sakSettings->setValue("language", language);
    }

    int exitCode = 0;
    do {
        int uiType = sakSettings->uiType();
        QCoreApplication *app{nullptr};
        if (uiType == SAKSettings::UiTypeWidget) {
            app = new SAKApplication(argc, argv);
        } else {
            app = new SAKGuiApplication(argc, argv);
        }

        auto sakI18n = SAKI18N::instance();
        sakI18n->setConfigurationFile(":/res/i18n/easydebug.json");
        sakI18n->installTranslator(language);


        // If the exit code is equal to SAK_REBOOT_CODE, the application will
        // be reboot.
        exitCode = app->exec();
        app->deleteLater();
#ifdef SAK_REBOOT_CODE
    }while (exitCode == SAK_REBOOT_CODE);
#else
    }while (exitCode == 1314);
#endif
    return exitCode;
}
