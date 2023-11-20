/*******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
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
#ifdef SAK_IMPORT_MODULE_PRIVATE
    QCoreApplication::setApplicationName(QString("QtSwissArmyKnife"));
#else
    QCoreApplication::setApplicationName(QString("QtSwissArmyKnife(Community)"));
#endif
#ifdef SAK_VERSION
    QCoreApplication::setApplicationVersion(SAK_VERSION);
#else
    QCoreApplication::setApplicationVersion("0.0.0");
#endif

    // echo hello world
    qDebug() << "Hello world!";

    QLoggingCategory lc{"SAK.Main"};

    // Remove settings file and database
    if (SAKSettings::instance()->clearSettings()) {
        SAKSettings::instance()->setClearSettings(false);
        if (QFile::remove(SAKSettings::instance()->fileName())) {
            qCInfo(lc) << "Remove settings file successfully.";
        } else {
            qCWarning(lc) << "Remove settings file failed!";
        }
    }

    qCInfo(lc) << "Supported style:" << QStyleFactory::keys();

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if (int policy = SAKSettings::instance()->hdpiPolicy();
        SAKInterface::isQtHighDpiScalePolicy(policy)) {
        const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif

    if (const QString style = SAKSettings::instance()->appStyle();
        SAKSettings::instance()->appStyle().isEmpty() && QStyleFactory::keys().contains(style)) {
        qCInfo(lc) << "App style:" << style;
        QApplication::setStyle(QStyleFactory::create(style));
    }
    const SAKApplication app(argc, argv);
    Q_UNUSED(app)
    SAKLog::instance()->start();
    return SAKApplication::exec();
}
