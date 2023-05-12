/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPSERVERTOOLBOXUI_HH
#define SAKUDPSERVERTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKUdpServerToolBoxUiController.hh"

class SAKUdpServerToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKUdpServerToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKUdpServerToolBoxUiController *mController;
};

#endif // SAKUDPSERVERTOOLBOXUI_HH
