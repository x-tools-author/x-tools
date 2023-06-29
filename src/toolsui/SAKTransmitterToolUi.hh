/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTRANSMITTERTOOLUI_HH
#define SAKTRANSMITTERTOOLUI_HH

#include "SAKTableModelToolUi.hh"

class SAKTransmitterToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    SAKTransmitterToolUi(const char *lg, QWidget *parent = nullptr);

protected:
    virtual void afterRowEdited(int row) override;
};

#endif // SAKTRANSMITTERTOOLUI_HH
