/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKWEBSOCKETTRANSMITTERTOOLUI_H
#define SAKWEBSOCKETTRANSMITTERTOOLUI_H

#include "saksocketclienttransmittertoolui.h"

class SAKWebSocketTransmitterToolUi : public SAKSocketClientTransmitterToolUi
{
    Q_OBJECT
public:
    SAKWebSocketTransmitterToolUi(QWidget *parent = nullptr);

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup) override;
};

#endif // SAKWEBSOCKETTRANSMITTERTOOLUI_H
