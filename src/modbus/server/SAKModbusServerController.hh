/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSSERVERCONTROLLERSERVER_HH
#define SAKMODBUSSERVERCONTROLLERSERVER_HH

#include <SAKModbusCommonController.hh>

class SAKModbusServerController : public SAKModbusCommonController
{
    Q_OBJECT
public:
    SAKModbusServerController(QWidget *parent = Q_NULLPTR);
};

#endif // SAKMODBUSSERVERCONTROLLERSERVER_HH
