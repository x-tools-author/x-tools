/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKUDPDEBUGPAGE_HH
#define SAKUDPDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKUdpDevice;
class SAKUdpDeviceController;
class SAKUdpDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKUdpDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKUdpDebugPage();

    /**
     * @brief controllerInstance 获取
     * @return
     */
    SAKUdpDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKUdpDeviceController *udpDeviceController;
};

#endif
