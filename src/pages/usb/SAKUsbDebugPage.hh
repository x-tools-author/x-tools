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
#ifndef SAKUSBDEBUGPAGE_HH
#define SAKUSBDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKUsbDevice;
class SAKUsbDeviceController;
class SAKUsbDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKUsbDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKUsbDebugPage();

    /**
     * @brief UsbDeviceControllerInstance
     * @return
     */
    SAKUsbDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool ebable) final;
private:
    SAKUsbDeviceController *controller;
};

#endif
