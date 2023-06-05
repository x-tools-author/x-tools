/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSERIALPORTTRANSMITTERTOOL_HH
#define SAKSERIALPORTTRANSMITTERTOOL_HH

#include "SAKTransmitterTool.hh"

class SAKSerialPortTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKSerialPortTransmitterTool(QObject *parent = nullptr);
};

#endif // SAKSERIALPORTTRANSMITTERTOOL_HH
