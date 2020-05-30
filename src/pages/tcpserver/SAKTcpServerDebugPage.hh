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
#ifndef SAKTCPSERVERDEBUGPAGE_HH
#define SAKTCPSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKTcpServerDevice;
class SAKTcpServerDeviceController;
class SAKTcpServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKTcpServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例指针
     * @return 控制类实例
     */
    SAKTcpServerDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDevice* createDevice() final;
    void setUiEnable(bool enable);
private:
    SAKTcpServerDeviceController *tcpServerDeviceController;
};

#endif
