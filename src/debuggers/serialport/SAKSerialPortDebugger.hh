/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSERIALPORTDEFBUGGER_HH
#define SAKSERIALPORTDEFBUGGER_HH

#include "SAKDebugger.hh"

class SAKDebuggerDevice;
class SAKDebugPageController;
class SAKSerialPortController;
class SAKSerialPortDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKSerialPortDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() override;
    SAKDebugPageController *controller() override;
private:
    SAKDebuggerDevice *mDevice;
    SAKDebugPageController *mDeviceController;
};

#endif
