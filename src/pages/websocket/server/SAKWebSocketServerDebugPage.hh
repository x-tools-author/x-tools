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
/// @brief Web socket server debugging page
class SAKWebSocketServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketServerDebugPage(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
