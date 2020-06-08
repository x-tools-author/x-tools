/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QLocale>
#include <QDateTime>

#include "SAK.hh"

SAK *SAK::thisInstance = Q_NULLPTR;
static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
SAK::SAK(QObject *parent)
    :QObject (parent)
{
    thisInstance = this;
}

SAK::~SAK()
{
    thisInstance = Q_NULLPTR;
}

SAK* SAK::instance()
{
    if (!thisInstance){
        new SAK;
    }

    return thisInstance;
}

QString SAK::email()
{
    return QString("qsak@foxmail.com");
}

QString SAK::qqGroupNumber()
{
    return QString("952218522");
}

QString SAK::authorName()
{
    return QString("Qter");
}

QString SAK::authorNickname()
{
    return QString("Qt开发小王子");
}

QString SAK::version()
{
    return QString("3.0.0");
}

QString SAK::buildTime()
{
    return (buildDate.toString("yyyy/MM/dd") + " " + QString(__TIME__));
}

QString SAK::officeUrl()
{
    return QString("https://qsak.pro");
}

QString SAK::qqNumber()
{
    return QString("2869470394");
}

QString SAK::copyright()
{
    return QString("Copyright 2018-%1 Qter. All rights reserved").arg(buildDate.toString("yyyy"));
}

QString SAK::business()
{
    return tr("上位机定制开发，可联系作者洽谈");
}
