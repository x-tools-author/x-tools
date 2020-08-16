/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETCLIENTDEBUGPAGE_HH
#define SAKSSLSOCKETCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKSslSocketClientDeviceController;
class SAKSslSocketClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKSslSocketClientDebugPage(QWidget *parent = Q_NULLPTR);

    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice* createDevice() final;
private:
    SAKSslSocketClientDeviceController *mTcpClientDeviceController;
};

#endif
