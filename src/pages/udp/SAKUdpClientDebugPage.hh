/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    SAKDebugPageDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKUdpDeviceController *udpDeviceController;
};

#endif
