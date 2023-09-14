/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTRANSMITTERTOOLUI_HH
#define SAKSOCKETCLIENTTRANSMITTERTOOLUI_HH

#include "SAKTransmitterToolUi.h"
#include "SAKSocketClientTransmitterToolUiEditor.h"

class SAKSocketClientTransmitterToolUi : public SAKTransmitterToolUi
{
public:
    SAKSocketClientTransmitterToolUi(const char *lg, QWidget *parent = nullptr);
    virtual QDialog *itemEditor() override;

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingGroup) override;

protected:
    SAKSocketClientTransmitterToolUiEditor *mEditor{nullptr};
};

#endif // SAKSOCKETCLIENTTRANSMITTERTOOLUI_HH
