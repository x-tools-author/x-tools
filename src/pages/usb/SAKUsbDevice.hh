/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKUsbDEVICE_HH
#define SAKUsbDEVICE_HH

#include <QTimer>
#include <QThread>

#include "SAKDevice.hh"

extern "C" {
#include "hidapi.h"
}

class SAKUsbDebugPage;
class SAKUsbDevice:public SAKDevice
{
    Q_OBJECT
public:
    SAKUsbDevice(SAKUsbDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKUsbDevice();
private:
    void run();    
private:    
    SAKUsbDebugPage *debugPage;
    QString path;
    hid_device *hidDevice;
};

#endif
