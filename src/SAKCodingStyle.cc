/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>

#include "SAKCodingStyle.hh"

SAKCodingStyle::SAKCodingStyle(QObject *parent)
    :QObject (parent)
    ,obj1 (Q_NULLPTR)
    ,obj2 (Q_NULLPTR)
    ,obj3 (Q_NULLPTR)
{

}

void SAKCodingStyle::helloWorld()
{

}

QString SAKCodingStyle::stringProperty1()
{
    return _stringProperty1;
}

void SAKCodingStyle::setStringProperty1(QString str)
{
    _stringProperty1 = str;
    emit stringProperty1Changed();
}
