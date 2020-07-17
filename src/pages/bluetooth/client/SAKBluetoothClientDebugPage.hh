/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKBLUETOOTHCLIENTDEBUGPAGE_HH
#define SAKBLUETOOTHCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKBluetoothClientDevice;
class SAKBluetoothClientDeviceController;
class SAKBluetoothClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKBluetoothClientDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothClientDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例
     * @return 控制类实例
     */
    SAKBluetoothClientDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKBluetoothClientDeviceController *tcpClientDeviceController;
};

#endif
