/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETSERVERTOOLBOXUICONTROLLER_HH
#define SAKWEBSOCKETSERVERTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKWebSocketServerToolBoxUiController;
}

class SAKWebSocketServerToolBoxUiController  : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKWebSocketServerToolBoxUiController (QWidget *parent = nullptr);
    ~SAKWebSocketServerToolBoxUiController ();

private:
    Ui::SAKWebSocketServerToolBoxUiController  *ui;
};

#endif // SAKWEBSOCKETSERVERTOOLBOXUICONTROLLER_HH
