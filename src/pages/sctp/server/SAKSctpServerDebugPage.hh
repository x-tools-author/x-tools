/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSCTPSERVERDEBUGPAGE_HH
#define SAKSCTPSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKSctpServerDevice;
class SAKSctpServerDeviceController;
class SAKSctpServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKSctpServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKSctpServerDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例指针
     * @return 控制类实例
     */
    SAKSctpServerDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable);
private:
    SAKSctpServerDeviceController *tcpServerDeviceController;
};

#endif
