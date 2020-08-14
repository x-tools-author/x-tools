/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTDEBUGPAGE_HH
#define SAKWEBSOCKETCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKDebugPageController;
class SAKWebSocketClientDevice;
class SAKWebSocketClientDeviceController;
class SAKWebSocketClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKWebSocketClientDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketClientDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例
     * @return 控制类实例
     */
    SAKWebSocketClientDeviceController *controllerInstance();
protected:
    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice* device() final;
private:
    SAKWebSocketClientDeviceController *webSocketClientDeviceController;
};

#endif
