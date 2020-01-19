/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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

private:
    SAKTcpClientDevice *tcpClientDevice;
    SAKTcpClientDeviceController *tcpClientDeviceController;

    void setUiEnable(bool enable);
    void changeDeviceStatus(bool opened);
private:
    void openOrColoseDevice() final;
    void refreshDevice() final;
    QWidget *controllerWidget() final;
};

#endif
