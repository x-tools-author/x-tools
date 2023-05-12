/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPCLIENTTOOLBOXUICONTROLLER_HH
#define SAKUDPCLIENTTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKUdpClientToolBoxUiController;
}

class SAKUdpClientToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKUdpClientToolBoxUiController(QWidget *parent = nullptr);
    ~SAKUdpClientToolBoxUiController();

private:
    Ui::SAKUdpClientToolBoxUiController *ui;
};

#endif // SAKUDPCLIENTTOOLBOXUICONTROLLER_HH
