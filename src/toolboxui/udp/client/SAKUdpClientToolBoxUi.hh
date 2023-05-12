/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPCLIENTTOOLBOXUI_HH
#define SAKUDPCLIENTTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKUdpClientToolBoxUiController.hh"

class SAKUdpClientToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKUdpClientToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKUdpClientToolBoxUiController *mController;
};

#endif // SAKUDPCLIENTTOOLBOXUI_HH
