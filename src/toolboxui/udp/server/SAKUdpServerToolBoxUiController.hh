/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPSERVERTOOLBOXUICONTROLLER_HH
#define SAKUDPSERVERTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKUdpServerToolBoxUiController ;
}

class SAKUdpServerToolBoxUiController  : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKUdpServerToolBoxUiController (QWidget *parent = nullptr);
    ~SAKUdpServerToolBoxUiController ();

private:
    Ui::SAKUdpServerToolBoxUiController *ui;
};

#endif // SAKUDPSERVERTOOLBOXUICONTROLLER_HH
