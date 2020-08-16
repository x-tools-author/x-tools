/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETSERVERDEBUGPAGE_HH
#define SAKSSLSOCKETSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKSslSocketServerDevice;
class SAKSslSocketServerDeviceController;
class SAKSslSocketServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKSslSocketServerDebugPage(QWidget *parent = Q_NULLPTR);

    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice* createDevice() final;
private:
    SAKSslSocketServerDeviceController *mDeviceController;
};

#endif
