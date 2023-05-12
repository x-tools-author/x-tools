/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETCLIENTTOOLBOXUI_HH
#define SAKWEBSOCKETCLIENTTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKWebSocketClientToolBoxUiController.hh"

class SAKWebSocketClientToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKWebSocketClientToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKWebSocketClientToolBoxUiController *mController;
};

#endif // SAKWEBSOCKETCLIENTTOOLBOXUI_HH
