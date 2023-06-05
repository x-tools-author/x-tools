/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPTRANSMITTERTOOL_HH
#define SAKUDPTRANSMITTERTOOL_HH

#include "SAKTransmitterTool.hh"

class SAKUdpTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKUdpTransmitterTool(QObject *parent = nullptr);
};

#endif // SAKUDPTRANSMITTERTOOL_HH
