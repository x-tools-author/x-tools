/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTCPTRANSMITTERTOOL_H
#define SAKTCPTRANSMITTERTOOL_H

#include "saksocketclienttransmittertool.h"

class SAKTcpTransmitterTool : public SAKSocketClientTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKTcpTransmitterTool(QObject *parent = nullptr);
    virtual SAKCommunicationTool *createTool() override;
};

#endif // SAKTCPTRANSMITTERTOOL_H
