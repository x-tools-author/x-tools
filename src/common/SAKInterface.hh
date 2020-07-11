/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKINTERFACE_HH
#define SAKINTERFACE_HH

#include <QObject>

/// @brief 该类提供了一些接口，用来替代新版本Qt中引入的函数，目的是使得旧版本Qt也能编译本工程
class SAKInterface:public QObject
{
    Q_OBJECT
public:
    SAKInterface(QObject *parent = Q_NULLPTR);

    QByteArray byteArrayToHex(QByteArray &array, char separator);
};

#endif
