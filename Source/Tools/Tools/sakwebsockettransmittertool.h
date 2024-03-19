/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKWEBSOCKETTRANSMITTERTOOL_H
#define SAKWEBSOCKETTRANSMITTERTOOL_H

#include "saksocketclienttransmittertool.h"

class SAKWebSocketTransmitterTool : public SAKSocketClientTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKWebSocketTransmitterTool(QObject *parent = nullptr);
    virtual SAKCommunicationTool *createTool() override;
};

#endif // SAKWEBSOCKETTRANSMITTERTOOL_H
