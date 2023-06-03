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

#include "SAKTranslator.hh"
#include "SAKSettings.hh"
#include "SAKApplication.hh"

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include "SAKGuiApplication.hh"
#endif

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

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto policy = SAKSettings::instance()->hdpiPolicy();
    auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
#endif

    // Install language packet for the application.
    QString language = SAKSettings::instance()->language();
    SAKTranslator::instance()->installTranslator(language);

    // Startup application.
    if (SAKSettings::instance()->uiType() == SAKSettings::UiTypeWidget) {
        QString style = SAKSettings::instance()->appStyle();
        if (!style.isEmpty() && QStyleFactory::keys().contains(style)) {
            qCInfo(lc) << "App style:" << style;
            QApplication::setStyle(QStyleFactory::create(style));
        }

        SAKApplication app(argc, argv);
        app.exec();
    } else {
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
        SAKGuiApplication app(argc, argv);
        app.exec();
#else
        SAKSettings::instance()->setUiType(SAKSettings::UiTypeWidget);
        qInfo() << "Please using Qt 6.4.0 or later!";
#endif
    }
}
