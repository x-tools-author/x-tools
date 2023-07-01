/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSRTUSERIALDEVICE_HH
#define SAKMODBUSRTUSERIALDEVICE_HH

#include "SAKModbusDevice.hh"

class SAKModbusRtuSerialDevice : public SAKModbusDevice
{
    Q_OBJECT
public:
    explicit SAKModbusRtuSerialDevice(const char *lc,
                                      QObject *parent = Q_NULLPTR);
};

#endif // SAKMODBUSRTUSERIALDEVICE_HH
