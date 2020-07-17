/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKBLUETOOTHSERVERDEBUGPAGE_HH
#define SAKBLUETOOTHSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKBluetoothServerDevice;
class SAKBluetoothServerDeviceController;
class SAKBluetoothServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKBluetoothServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothServerDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例指针
     * @return 控制类实例
     */
    SAKBluetoothServerDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable);
private:
    SAKBluetoothServerDeviceController *tcpServerDeviceController;
};

#endif
