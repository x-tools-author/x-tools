/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPSERVERTOOLBOXUICONTROLLER_HH
#define SAKTCPSERVERTOOLBOXUICONTROLLER_HH

#include "SAKToolBoxUiController.hh"

namespace Ui {
class SAKTcpServerToolBoxUiController ;
}

class SAKTcpServerToolBoxUiController  : public SAKToolBoxUiController
{
    Q_OBJECT
public:
    explicit SAKTcpServerToolBoxUiController (QWidget *parent = nullptr);
    ~SAKTcpServerToolBoxUiController ();

private:
    Ui::SAKTcpServerToolBoxUiController  *ui;
};

#endif // SAKTCPSERVERTOOLBOXUICONTROLLER_HH
