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

class SAKDebugPageDevice;
class SAKDebugPageController;
class SAKSerialPortDeviceController;
class SAKSerialPortDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKSerialPortDebugPage(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
