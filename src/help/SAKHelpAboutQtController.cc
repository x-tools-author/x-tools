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
#include <QDesktopServices>

#include "SAKHelpAboutQtController.hh"

SAKHelpAboutQtController::SAKHelpAboutQtController(QObject *parent)
    :QObject (parent)
{

}

SAKHelpAboutQtController::~SAKHelpAboutQtController()
{

}

void SAKHelpAboutQtController::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

QString SAKHelpAboutQtController::officialRelease()
{
    return QString("http://download.qt.io/official_releases");
}

QString SAKHelpAboutQtController::officialReleaseWiki()
{
    return QString("https://wiki.qt.io/Qt_5.15_Release");
}

QString SAKHelpAboutQtController::officialBlog()
{
    return QString("https://www.qt.io/blog");
}

QString SAKHelpAboutQtController::officialWeb()
{
    return QString("https://www.qt.io/");
}

QString SAKHelpAboutQtController::chineseRelease()
{
    return QString("https://mirrors.tuna.tsinghua.edu.cn/qt");
}

QString SAKHelpAboutQtController::qtVersion()
{
    return QT_VERSION_STR;
}
