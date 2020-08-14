/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETSERVERDEBUGPAGE_HH
#define SAKWEBSOCKETSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKDebugPageController;
class SAKWebSocketServerDevice;
class SAKWebSocketServerDeviceController;
/// @brief web socket服务器调试页面
class SAKWebSocketServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKWebSocketServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketServerDebugPage();

    /**
     * @brief controllerInstance 获取控制类实例指针
     * @return 控制类实例
     */
    SAKWebSocketServerDeviceController *controllerInstance();
protected:
    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice* device() final;
    void setUiEnable(bool enable);
private:
    SAKWebSocketServerDeviceController *tcpServerDeviceController;
};

#endif
