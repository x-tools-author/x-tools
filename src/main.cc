/***************************************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the
 * source code directory.
 **************************************************************************************************/
#include <QFile>
#include <QStyleFactory>

#include "sakapplication.h"
#include "sakinterface.h"
#include "saklog.h"
#include "saksettings.h"

int main(const int argc, char* argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(SAKLog::messageOutput);
#endif

    // Initialize some information about application.
    QCoreApplication::setOrganizationName(QString("Qsaker"));
    QCoreApplication::setOrganizationDomain(QString("IT"));

    QString appName = QString("QtSwissArmyKnife");
#ifndef SAK_IMPORT_MODULE_PRIVATE
    appName += QObject::tr("(Community)");
#endif

#ifdef SAK_VERSION
    QCoreApplication::setApplicationVersion(SAK_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif

    SAKApplication app(argc, argv);
    SAKLog::instance()->start();

    // Remove settings file and database
    if (SAKSettings::instance()->clearSettings()) {
        SAKSettings::instance()->setClearSettings(false);
        if (QFile::remove(SAKSettings::instance()->fileName())) {
            qInfo() << "Remove settings file successfully.";
        } else {
            qWarning() << "Remove settings file failed!";
        }
    }

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if (int policy = SAKSettings::instance()->hdpiPolicy();
        SAKInterface::isQtHighDpiScalePolicy(policy)) {
        const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif

    // Application style.
    qInfo() << "Supported style:" << QStyleFactory::keys();
    if (const QString style = SAKSettings::instance()->appStyle();
        QStyleFactory::keys().contains(style)) {
        qInfo() << "App style:" << style;
        QApplication::setStyle(QStyleFactory::create(style));
    }

    return app.exec();
}
