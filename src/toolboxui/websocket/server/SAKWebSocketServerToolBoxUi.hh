/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETSERVERTOOLBOXUI_HH
#define SAKWEBSOCKETSERVERTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKWebSocketServerToolBoxUiController.hh"

class SAKWebSocketServerToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKWebSocketServerToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKWebSocketServerToolBoxUiController *mController;
};

#endif // SAKWEBSOCKETSERVERTOOLBOXUI_HH
