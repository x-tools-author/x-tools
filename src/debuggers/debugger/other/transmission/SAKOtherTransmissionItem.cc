/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugger.hh"
#include "SAKOtherTransmissionItem.hh"

SAKOtherTransmissionItem::SAKOtherTransmissionItem(SAKDebugger *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
{
    connect(mDebugPage, &SAKDebugger::bytesRead, this, &SAKOtherTransmissionItem::write);
    connect(this, &SAKOtherTransmissionItem::bytesRead, mDebugPage, &SAKDebugger::write);
}

void SAKOtherTransmissionItem::write(QByteArray data)
{
    Q_UNUSED(data);
}
