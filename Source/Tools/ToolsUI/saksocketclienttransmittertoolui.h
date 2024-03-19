/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSOCKETCLIENTTRANSMITTERTOOLUI_H
#define SAKSOCKETCLIENTTRANSMITTERTOOLUI_H

#include "saksocketclienttransmittertooluieditor.h"
#include "saktransmittertoolui.h"

class SAKSocketClientTransmitterToolUi : public SAKTransmitterToolUi
{
public:
    SAKSocketClientTransmitterToolUi(const char *lg, QWidget *parent = nullptr);
    virtual QDialog *itemEditor() override;

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup) override;

protected:
    SAKSocketClientTransmitterToolUiEditor *mEditor{nullptr};
};

#endif // SAKSOCKETCLIENTTRANSMITTERTOOLUI_H
