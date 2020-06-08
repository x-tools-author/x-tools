/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
