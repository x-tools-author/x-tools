/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTCPTRANSMITTERTOOLTI_H
#define SAKTCPTRANSMITTERTOOLTI_H

#include "saksocketclienttransmittertoolui.h"

class SAKTcpTransmitterToolUi : public SAKSocketClientTransmitterToolUi
{
public:
    explicit SAKTcpTransmitterToolUi(QWidget *parent = nullptr);
};

#endif // SAKTCPTRANSMITTERTOOLTI_H
