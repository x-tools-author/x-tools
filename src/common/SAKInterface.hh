/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
