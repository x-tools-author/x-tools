/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTCPCLIENTDEBUGPAGE_HH
#define SAKTCPCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKTcpClientDevice;
class SAKTcpClientDeviceController;
class SAKTcpClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKTcpClientDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKTcpClientDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例
     * @return 控制类实例
     */
    SAKTcpClientDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKTcpClientDeviceController *tcpClientDeviceController;
};

#endif
