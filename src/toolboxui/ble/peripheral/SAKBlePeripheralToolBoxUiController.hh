/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLEPERIPHERALTOOLBOXUICONTROLLER_HH
#define SAKBLEPERIPHERALTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKBlePeripheralToolBoxUiController;
}

class SAKBlePeripheralToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKBlePeripheralToolBoxUiController(QWidget *parent = nullptr);
    ~SAKBlePeripheralToolBoxUiController();

private:
    Ui::SAKBlePeripheralToolBoxUiController *ui;
};

#endif // SAKBLEPERIPHERALTOOLBOXUICONTROLLER_HH
