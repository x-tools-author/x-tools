/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLEPERIPHERALTOOLBOXUI_HH
#define SAKBLEPERIPHERALTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKBlePeripheralToolBoxUiController.hh"

class SAKBlePeripheralToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKBlePeripheralToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKBlePeripheralToolBoxUiController *mController;
};

#endif // SAKBLEPERIPHERALTOOLBOXUICONTROLLER_HH
