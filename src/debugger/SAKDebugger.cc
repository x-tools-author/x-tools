/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKApplication.hh"

SAKApplication::SAKApplication(int argc, char **argv)
    : QGuiApplication (argc, argv)
{

}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::setupUi()
{
    qmlAppEngine.load(QUrl(QString("qrc:/qml/MainWindow.qml")));
}
