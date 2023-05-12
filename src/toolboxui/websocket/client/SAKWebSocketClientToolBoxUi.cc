/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKWebSocketClientToolBoxUi.hh"

SAKWebSocketClientToolBoxUi::SAKWebSocketClientToolBoxUi(QWidget *parent)
    : SAKToolBoxUi(parent)
    , mController(new SAKWebSocketClientToolBoxUiController)
{
    setWindowTitle(tr("WS-C"));
    init();
}

QWidget *SAKWebSocketClientToolBoxUi::controller()
{
    return mController;
}
