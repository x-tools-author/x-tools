/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
