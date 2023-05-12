/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALTOOLBOXUICONTROLLER_HH
#define SAKBLECENTRALTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKBleCentralToolBoxUiController;
}

class SAKBleCentralToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKBleCentralToolBoxUiController(QWidget *parent = nullptr);
    ~SAKBleCentralToolBoxUiController();

private:
    Ui::SAKBleCentralToolBoxUiController *ui;
};

#endif // SAKBLECENTRALTOOLBOXUICONTROLLER_HH
