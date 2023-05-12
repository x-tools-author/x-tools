/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUISERIALPORTCONTROLLER_H
#define SAKTOOLBOXUISERIALPORTCONTROLLER_H

#include "SAKToolBoxUiBaseController.hh"

namespace Ui {
class SAKToolBoxUiSerialPortController;
}

class SAKToolBoxUiSerialPortController : public SAKToolBoxUiBaseController
{
    Q_OBJECT
public:
    explicit SAKToolBoxUiSerialPortController(SAKCommunicationTool *tool,
                                              QWidget *parent = nullptr);
    ~SAKToolBoxUiSerialPortController();

private:
    Ui::SAKToolBoxUiSerialPortController *ui;
};

#endif // SAKTOOLBOXUISERIALPORTCONTROLLER_H
