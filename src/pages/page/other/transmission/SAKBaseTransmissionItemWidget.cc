/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKDebugPage.hh"
#include "SAKBaseTransmissionItemWidget.hh"

SAKBaseTransmissionItemWidget::SAKBaseTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,debugPage (debugPage)
{
    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKBaseTransmissionItemWidget::write);
    connect(this, &SAKBaseTransmissionItemWidget::bytesRead, debugPage, &SAKDebugPage::write);
}

void SAKBaseTransmissionItemWidget::write(QByteArray data)
{
    Q_UNUSED(data);
}
