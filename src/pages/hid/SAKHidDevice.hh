/*
 * Copyright (C) 2019-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKHIDDEVICE_HH
#define SAKHIDDEVICE_HH

#include <QTimer>
#include <QThread>

#include "SAKDevice.hh"

extern "C" {
#include "hidapi.h"
}

class SAKHidDebugPage;
class SAKHidDevice:public SAKDevice
{
    Q_OBJECT
public:
    SAKHidDevice(SAKHidDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKHidDevice();
private:
    void run();    
private:    
    SAKHidDebugPage *debugPage;
    QString path;
    hid_device *hidDevice;
};

#endif
