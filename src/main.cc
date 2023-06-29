/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QStyleFactory>
#include <QLoggingCategory>

#include "SAKLog.hh"
#include "SAKSettings.hh"
#include "SAKInterface.hh"
#include "SAKApplication.hh"

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include "SAKGuiApplication.hh"
#endif

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(SAKLog::messageOutput);
#endif

    // Initialize some information about application.
    QCoreApplication::setOrganizationName(QString("Qsaker"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(QString("QtSwissArmyKnife"));
#ifdef SAK_VERSION
    QCoreApplication::setApplicationVersion(SAK_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif

    QLoggingCategory lc{"sak.main"};

    // Remove settings file and database
    if (SAKSettings::instance()->clearSettings()){
        SAKSettings::instance()->setClearSettings(false);
        if (QFile::remove(SAKSettings::instance()->fileName())){
            qCInfo(lc) << "remove settings file successfully.";
        }else{
            qCWarning(lc) << "remove settings file failed!";
        }
    }

    qCInfo(lc) << "supported style:" << QStyleFactory::keys();

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = SAKSettings::instance()->hdpiPolicy();
    if (SAKInterface::isQtHighDpiScalePolicy(policy)) {
        auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif

    // Startup application.
    if (SAKSettings::instance()->uiType() == SAKSettings::UiTypeWidget) {
        QString style = SAKSettings::instance()->appStyle();
        if (!style.isEmpty() && QStyleFactory::keys().contains(style)) {
            qCInfo(lc) << "app style:" << style;
            QApplication::setStyle(QStyleFactory::create(style));
        }

        SAKApplication app(argc, argv);
        SAKLog::instance()->start();
        return app.exec();
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    SAKGuiApplication app(argc, argv);
    SAKLog::instance()->start();
    return app.exec();
#else
    SAKSettings::instance()->setUiType(SAKSettings::UiTypeWidget);
    qCInfo(lc) << "please using Qt 6.4.0 or later!";
    return 0;
#endif
}
