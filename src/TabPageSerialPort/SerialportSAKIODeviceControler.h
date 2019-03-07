/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SERIALPORTSAKIODEVICECONTROLER_H
#define SERIALPORTSAKIODEVICECONTROLER_H

#include "SAKIODeviceControler.h"

namespace Ui{
class SerialportSAKIODeviceControler;
}

class SerialportSAKIODeviceControler: public SAKIODeviceControler
{
    Q_OBJECT
public:
    SerialportSAKIODeviceControler(QWidget *parent = Q_NULLPTR);
    ~SerialportSAKIODeviceControler();

    virtual void afterDeviceOpen();
    virtual void afterDeviceClose();
public slots:
    virtual void open();
    virtual void refresh();
private:
    Ui::SerialportSAKIODeviceControler *ui = nullptr;
    /// -----------------------------------------------
    void initUI();
};

#endif
