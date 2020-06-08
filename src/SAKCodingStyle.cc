/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
