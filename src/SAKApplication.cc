/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QTimer>
#include <QAction>
#include <QSettings>
#include <QTextCursor>
#include <QTranslator>

#include "SAK.hh"
#include "SAKSettings.hh"
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"

SAKApplication::SAKApplication(int argc, char **argv)
    : QApplication (argc, argv)
{
    SAKSplashScreen *splashScreen = SAKSplashScreen::instance();
    splashScreen->show();
    processEvents();

    setApplicationVersion(SAK_VERSION);
    SAKSettings::instance();
    installLanguage();
    setApplicationVersion(SAK::instance()->version());

    /// @brief 注册表选项
    setOrganizationName(QString("Qter"));
    setOrganizationDomain(QString("IT"));
    setApplicationName(QString("QtSwissArmyKnife"));

    QTimer::singleShot(5*1000, [=](){
        if (SAKSettings::instance()->enableAutoCheckForUpdate()){
            emit this->checkForUpdate();
        }
    });

    QMainWindow *mainWindow = new SAKMainWindow;
    mainWindow->show();
    splashScreen->finish(mainWindow);
}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::installLanguage()
{
    QString language = SAKSettings::instance()->language();
    QString qmName;
    if (language.isEmpty()){
        if (QLocale().country() == QLocale::China){
            qmName = QString("zh_CN");
        }else{
            qmName = QString("en");
        }
    }else{
        qmName = language.split('-').first();
    }

    qtBaeTranslator.load(QString(":/translations/qt/qtbase_%1.qm").arg(qmName));
    qApp->installTranslator(&qtBaeTranslator);

    qtTranslator.load(QString(":/translations/qt/qt_%1.qm").arg(qmName));
    qApp->installTranslator(&qtTranslator);

    sakTranslator.load(QString(":/translations/sak/SAK_%1.qm").arg(qmName));
    qApp->installTranslator(&sakTranslator);

    if (sender() && inherits("QAction")){
        QAction *action = reinterpret_cast<QAction*>(sender());
        action->setChecked(true);
        QString title = action->data().toString();
    }
}
