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

#include "SAKTransmitterToolUi.hh"
#include "SAKTcpTransmitterToolUiEditor.hh"

class SAKTcpTransmitterToolUi : public SAKTransmitterToolUi
{
    Q_OBJECT
public:
    SAKTcpTransmitterToolUi(QWidget *parent = nullptr);
    virtual QDialog *itemEditor() override;

private:
    SAKTcpTransmitterToolUiEditor *mEditor{nullptr};
};

#endif // SAKTCPTRANSMITTERTOOLUI_HH
