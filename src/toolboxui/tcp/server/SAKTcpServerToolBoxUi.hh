/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPSERVERTOOLBOXUI_HH
#define SAKTCPSERVERTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKTcpServerToolBoxUiController.hh"

class SAKTcpServerToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKTcpServerToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKTcpServerToolBoxUiController *mController;
};

#endif // SAKTCPSERVERTOOLBOXUI_HH
