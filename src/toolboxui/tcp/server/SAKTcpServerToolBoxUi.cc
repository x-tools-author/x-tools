/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTcpServerToolBoxUi.hh"

SAKTcpServerToolBoxUi::SAKTcpServerToolBoxUi(QWidget *parent)
    : SAKToolBoxUi(parent)
    , mController(new SAKTcpServerToolBoxUiController)
{
    setWindowTitle(tr("TCP-S"));
    init();
}

QWidget *SAKTcpServerToolBoxUi::controller()
{
    return mController;
}
