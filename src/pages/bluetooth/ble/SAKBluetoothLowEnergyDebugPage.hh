/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHLOWENERGYTDEBUGPAGE_HH
#define SAKBLUETOOTHLOWENERGYDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKBluetoothLowEnergyDeviceController;
class SAKBluetoothLowEnergyDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKBluetoothLowEnergyDebugPage(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
