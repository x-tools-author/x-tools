/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QUrl>
#include <QDateTime>
#include <QDesktopServices>
#include <QCoreApplication>

#include "SAKHelpAboutSAKController.hh"

SAKHelpAboutSAKController::SAKHelpAboutSAKController(QObject *parent)
    :QObject (parent)
{

}

SAKHelpAboutSAKController::~SAKHelpAboutSAKController()
{

}

void SAKHelpAboutSAKController::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

QString SAKHelpAboutSAKController::appVersion()
{
    return qApp->applicationVersion();
}

QString SAKHelpAboutSAKController::authorName()
{
    return QString("Qter");
}

QString SAKHelpAboutSAKController::authorEmail()
{
    return QString("wuuhii@outlook.com");
}

QString SAKHelpAboutSAKController::buildDateTime()
{
    QString dateString(__DATE__);
    QString timeString(__TIME__);

    QString dateTimeString;
    dateTimeString.append(dateString);
    dateTimeString.append(" ");
    dateTimeString.append(timeString);

    return dateTimeString;
}

QString SAKHelpAboutSAKController::copyright()
{
    QString yyyy = QString(__DATE__).split(' ').last();
    QString info = QString("Copyright 2018-%1 Qter, All rights reserved.").arg(yyyy);
    return info;
}

QString SAKHelpAboutSAKController::githubUrl()
{
    return QString("https://github.com/wuuhii/QtSwissArmyKnife");
}

QString SAKHelpAboutSAKController::gitteeUrl()
{
    return QString("https://gitee.com/wuuhii/QtSwissArmyKnife");
}

QString SAKHelpAboutSAKController::officialRelease()
{
    return QString("https://qsak.pro");
}
