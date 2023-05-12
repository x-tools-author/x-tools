/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSERIALPORTTOOLBOXUICONTROLLER_HH
#define SAKSERIALPORTTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKSerialPortToolBoxUiController;
}

class SAKSerialPortToolBoxUiController : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKSerialPortToolBoxUiController(QWidget *parent = nullptr);
    ~SAKSerialPortToolBoxUiController();

private:
    Ui::SAKSerialPortToolBoxUiController *ui;
};

#endif // SAKSERIALPORTTOOLBOXUICONTROLLER_HH
