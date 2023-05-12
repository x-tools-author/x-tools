/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETCLIENTTOOLBOXUICONTROLLER_HH
#define SAKWEBSOCKETCLIENTTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKWebSocketClientToolBoxUiController;
}

class SAKWebSocketClientToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKWebSocketClientToolBoxUiController(QWidget *parent = nullptr);
    ~SAKWebSocketClientToolBoxUiController();

private:
    Ui::SAKWebSocketClientToolBoxUiController *ui;
};

#endif // SAKWEBSOCKETCLIENTTOOLBOXUICONTROLLER_HH
