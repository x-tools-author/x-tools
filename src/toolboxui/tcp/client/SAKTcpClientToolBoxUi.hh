/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPCLIENTTOOLBOXUI_HH
#define SAKTCPCLIENTTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKTcpClientToolBoxUiController.hh"

class SAKTcpClientToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKTcpClientToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKTcpClientToolBoxUiController *mController;
};

#endif // SAKTCPCLIENTTOOLBOXUI_HH
