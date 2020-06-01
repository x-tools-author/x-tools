/*
 * Copyright (C) 2019-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
