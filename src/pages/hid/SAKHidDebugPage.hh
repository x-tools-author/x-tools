/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKHIDDEBUGPAGE_HH
#define SAKHIDDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKHidDevice;
class SAKHidDeviceController;
class SAKHidDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKHidDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKHidDebugPage();

    /**
     * @brief hidDeviceControllerInstance
     * @return
     */
    SAKHidDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool ebable) final;
private:
    SAKHidDeviceController *controller;
};

#endif
