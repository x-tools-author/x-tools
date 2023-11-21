/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktransmittertoolui.h"
#include "saktransmittertool.h"

SAKTransmitterToolUi::SAKTransmitterToolUi(const char *lg, QWidget *parent)
    : SAKTableModelToolUi{lg, parent}
{}

void SAKTransmitterToolUi::afterRowEdited(int row)
{
    SAKTransmitterTool *cookedTool = Q_NULLPTR;
    cookedTool = qobject_cast<SAKTransmitterTool *>(mTool);
    if (!cookedTool) {
        return;
    }

    SAKCommunicationTool *tool = cookedTool->communicationTool(row);
    if (tool) {
        tool->exit();
        tool->wait();
        tool->start();
        qCInfo(mLoggingCategory) << "parameters changed, restart the thread";
    }
}
