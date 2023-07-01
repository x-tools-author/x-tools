/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSRTUSERIALSERVER_HH
#define SAKMODBUSRTUSERIALSERVER_HH

#include "SAKModbusServer.hh"

class SAKModbusRtuSerialServer : public SAKModbusServer
{
    Q_OBJECT
public:
    explicit SAKModbusRtuSerialServer(QObject *parent = Q_NULLPTR);
};

#endif // SAKMODBUSRTUSERIALSERVER_HH
