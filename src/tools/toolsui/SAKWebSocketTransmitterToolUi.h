/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKWEBSOCKETTRANSMITTERTOOLUI_HH
#define SAKWEBSOCKETTRANSMITTERTOOLUI_HH

#include "SAKSocketClientTransmitterToolUi.h"

class SAKWebSocketTransmitterToolUi : public SAKSocketClientTransmitterToolUi
{
    Q_OBJECT
public:
    SAKWebSocketTransmitterToolUi(QWidget *parent = nullptr);

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingGroup) override;
};

#endif // SAKWEBSOCKETTRANSMITTERTOOLUI_HH
