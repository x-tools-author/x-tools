/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEBUGPAGE_HH
#define SAKUDPCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKUdpClientDevice;
class SAKUdpClientDeviceController;
class SAKUdpClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKUdpClientDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientDebugPage();

    /**
     * @brief controllerInstance 获取
     * @return
     */
    SAKUdpClientDeviceController *controllerInstance();
protected:
    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice* device() final;
private:
    SAKUdpClientDeviceController *udpDeviceController;
};

#endif
