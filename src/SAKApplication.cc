/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKSettings.hh"
#include "SAKApplication.hh"
#include "SAKMainWindow.hh"

#include <QTimer>
#include <QSettings>
#include <QTextCursor>
#include <QTranslator>

SAKApplication::SAKApplication(int argc, char **argv):
    QApplication(argc, argv)
{
    installLanguage();
    setApplicationVersion(QString("2.2.0"));

    /// 注册表选项
    setOrganizationName(QString("Qter"));
    setOrganizationDomain(QString("IT"));
    setApplicationName(QString("QtSwissArmyKnife"));

    mpMainWindow = new SAKMainWindow;
    mpMainWindow->show();

    QTimer::singleShot(5*1000, [=](){
        if (SAKSettings::instance()->enableAutoCheckForUpdate()){
            emit this->checkForUpdate();
        }
    });
}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::installLanguage()
{
    QString language = SAKSettings::instance()->value(_settingStringLanguage).toString();
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

    if (sender()){
        QAction *action = reinterpret_cast<QAction*>(sender());
        action->setChecked(true);
        QString title = action->data().toString();
    }
}
