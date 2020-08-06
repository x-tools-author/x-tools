/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERDEBUGPAGE_HH
#define SAKUDPSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKUdpServerDevice;
class SAKUdpServerDeviceController;
class SAKUdpServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKUdpServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKUdpServerDebugPage();

    /**
     * @brief controllerInstance: Get the instance of device controller
     * @return: Instance of device controller
     */
    SAKUdpServerDeviceController *controllerInstance();
protected:
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    SAKDebugPageDevice* createDevice() final;
    void setUiEnable(bool enable) final;
private:
    SAKUdpServerDeviceController *mDeviceController;
};

#endif
