/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTABPAGESERIALPORTASSISTANT_HH
#define SAKTABPAGESERIALPORTASSISTANT_HH

#include "SAKDebugPage.hh"

class SAKSerialPortDeviceController;
class SAKSerialPortDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKSerialPortDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKSerialPortDebugPage();

    /**
     * @brief controllerInstance 获取设备控制面板实例
     * @return 设备控制面板实例
     */
    SAKSerialPortDeviceController *controllerInstance();
protected:
    SAKDevice *createDevice() final;
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    void setUiEnable(bool enable) final;
private:
    SAKSerialPortDeviceController *controller;
};

#endif
