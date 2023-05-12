/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALTOOLBOXUI_HH
#define SAKBLECENTRALTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKBleCentralToolBoxUiController.hh"

class SAKBleCentralToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKBleCentralToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKBleCentralToolBoxUiController *mController;
};

#endif // SAKBLECENTRALTOOLBOXUI_HH
