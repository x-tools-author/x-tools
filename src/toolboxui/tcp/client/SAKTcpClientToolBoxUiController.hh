/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPCLIENTTOOLBOXUICONTROLLER_HH
#define SAKTCPCLIENTTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKTcpClientToolBoxUiController;
}

class SAKTcpClientToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKTcpClientToolBoxUiController(QWidget *parent = nullptr);
    ~SAKTcpClientToolBoxUiController();

private:
    Ui::SAKTcpClientToolBoxUiController *ui;
};

#endif // SAKTCPCLIENTTOOLBOXUICONTROLLER_HH
