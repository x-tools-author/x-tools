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
#ifndef SAKCOMMOMINTERFACE_HH
#define SAKCOMMOMINTERFACE_HH

#include <QObject>

class SAKCommonInterface:public QObject
{
    /*
     * 该类提供了一些接口，用来替代新版本Qt中引入的函数，目的是使得旧版本Qt也能编译本工程
     */
    Q_OBJECT
public:
    SAKCommonInterface(QObject *parent = nullptr);

    QByteArray byteArrayToHex(QByteArray &array, char separator);
};

#endif
