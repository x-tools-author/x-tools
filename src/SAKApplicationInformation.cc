/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */

#include <QLocale>
#include <QDateTime>

#include "SAKApplicationInformation.hh"

SAKApplicationInformation *SAKApplicationInformation::thisInstance = nullptr;
static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
SAKApplicationInformation::SAKApplicationInformation(QObject *parent)
    :QObject (parent)
{
    thisInstance = this;
}

SAKApplicationInformation::~SAKApplicationInformation()
{
    thisInstance = nullptr;
}

SAKApplicationInformation* SAKApplicationInformation::instance()
{
    if (!thisInstance){
        new SAKApplicationInformation;
    }

    return thisInstance;
}

QString SAKApplicationInformation::email()
{
    return QString("wuuhii@outlook.com");
}

QString SAKApplicationInformation::qqGroupNumber()
{
    return QString("952218522");
}

QString SAKApplicationInformation::authorName()
{
    return QString("Qter");
}

QString SAKApplicationInformation::authorNickname()
{
    return QString("Qt开发小王子");
}

QString SAKApplicationInformation::version()
{
    return QString("2.3.0");
}

QString SAKApplicationInformation::buildTime()
{
    return (buildDate.toString("yyyy/MM/dd") + " " + QString(__TIME__));
}

QString SAKApplicationInformation::officeUrl()
{
    return QString("wuhai.pro");
}

QString SAKApplicationInformation::qqNumber()
{
    return QString("2869470394");
}

QString SAKApplicationInformation::copyright()
{
    return QString("Copyright(©) 2018-%1 Qter. All rights reserved").arg(buildDate.toString("yyyy"));
}

QString SAKApplicationInformation::business()
{
    return tr("软件定制开发，可联系作者");
}
