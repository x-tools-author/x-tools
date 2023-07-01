/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSTCPCLIENT_HH
#define SAKMODBUSTCPCLIENT_HH

#include "SAKModbusClient.hh"

class SAKModbusTcpClient : public SAKModbusClient
{
    Q_OBJECT
public:
    explicit SAKModbusTcpClient(QObject *parent = nullptr);
};

#endif // SAKMODBUSTCPCLIENT_HH
