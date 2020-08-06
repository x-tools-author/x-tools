/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSERIALPORTDEBUGPAGE_HH
#define SAKSERIALPORTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKSerialPortDeviceController;
class SAKSerialPortDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKSerialPortDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKSerialPortDebugPage();

    /**
     * @brief controllerInstance: Get the controller instance
     * @return Controller instance
     */
    SAKSerialPortDeviceController *controllerInstance();
protected:
    SAKDebugPageDevice *createDevice() final;
    void refreshDevice() final;
    QWidget *controllerWidget() final;
    void setUiEnable(bool enable) final;
private:
    SAKSerialPortDeviceController *controller;
};

#endif
