/******************************************************************************
 * Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QLoggingCategory>

#include "SAKLog.hh"
#include "SAKSettings.hh"
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

    QLoggingCategory lc{"SAK.main"};

    // Remove settings file and database
    if (SAKSettings::instance()->clearSettings()){
        SAKSettings::instance()->setClearSettings(false);
        if (QFile::remove(SAKSettings::instance()->fileName())){
            qCInfo(lc) << "Remove settings file successfully!";
        }else{
            qCWarning(lc) << "Remove settings file failed!";
        }
    }

    qCInfo(lc) << "supported style:" << QStyleFactory::keys();

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto policy = SAKSettings::instance()->hdpiPolicy();
    auto round = Qt::HighDpiScaleFactorRoundingPolicy::Round;
    auto ceil = Qt::HighDpiScaleFactorRoundingPolicy::Ceil;
    auto floor = Qt::HighDpiScaleFactorRoundingPolicy::Floor;
    auto rpf = Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor;
    auto passThrough = Qt::HighDpiScaleFactorRoundingPolicy::PassThrough;
    auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
    if (cookedPolicy == round || cookedPolicy == ceil
        || cookedPolicy == floor || cookedPolicy == rpf
        || cookedPolicy == passThrough) {
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif

    // Startup application.
    if (SAKSettings::instance()->uiType() == SAKSettings::UiTypeWidget) {
        QString style = SAKSettings::instance()->appStyle();
        if (!style.isEmpty() && QStyleFactory::keys().contains(style)) {
            qCInfo(lc) << "App style:" << style;
            QApplication::setStyle(QStyleFactory::create(style));
        }

        SAKApplication app(argc, argv);
        SAKLog::instance()->start();
        return app.exec();
    } else {
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
        SAKGuiApplication app(argc, argv);
        SAKLog::instance()->start();
        return app.exec();
#else
        SAKSettings::instance()->setUiType(SAKSettings::UiTypeWidget);
        qInfo() << "Please using Qt 6.4.0 or later!";
        return 0;
#endif
    }
}
