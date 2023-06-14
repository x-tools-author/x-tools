/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPTRANSMITTERTOOLUI_HH
#define SAKTCPTRANSMITTERTOOLUI_HH

#include "SAKSocketClientTool.hh"
#include "SAKSocketClientTransmitterToolUi.hh"

class SAKTcpTransmitterToolUi : public SAKSocketClientTransmitterToolUi
{
public:
    explicit SAKTcpTransmitterToolUi(QWidget *parent = nullptr);
};

#endif // SAKTCPTRANSMITTERTOOLUI_HH
