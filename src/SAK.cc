/*
 * Copyright (C) 2019-2020 Qter. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */

#include <QLocale>
#include <QDateTime>

#include "SAKApplicationInformation.hh"

SAKApplicationInformation *SAKApplicationInformation::thisInstance = Q_NULLPTR;
static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
SAKApplicationInformation::SAKApplicationInformation(QObject *parent)
    :QObject (parent)
{
    thisInstance = this;
}

SAKApplicationInformation::~SAKApplicationInformation()
{
    thisInstance = Q_NULLPTR;
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
    return QString("qsak@foxmail.com");
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
    return QString("3.0.0");
}

QString SAKApplicationInformation::buildTime()
{
    return (buildDate.toString("yyyy/MM/dd") + " " + QString(__TIME__));
}

QString SAKApplicationInformation::officeUrl()
{
    return QString("https://qsak.pro");
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
    return tr("上位机定制开发，可联系作者洽谈");
}
