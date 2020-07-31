/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    SAKDebugPageDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKTcpClientDeviceController *tcpClientDeviceController;
};

#endif
