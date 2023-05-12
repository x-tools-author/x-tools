/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSERIALPORTTOOLBOXUI_HH
#define SAKSERIALPORTTOOLBOXUI_HH

#include "SAKToolBoxUi.hh"
#include "SAKSerialPortToolBoxUiController.hh"

class SAKSerialPortToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKSerialPortToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKSerialPortToolBoxUiController *mController{nullptr};
};

#endif // SAKSERIALPORTTOOLBOXUI_H
