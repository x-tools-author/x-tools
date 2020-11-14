/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSSERVERCONTROLLERSERVER_HH
#define SAKMODBUSSERVERCONTROLLERSERVER_HH

#include <SAKModbusCommonController.hh>

class SAKModbusCommonServerSection;
class SAKModbusServerController : public SAKModbusCommonController
{
    Q_OBJECT
public:
    SAKModbusServerController(QWidget *parent = Q_NULLPTR);
    virtual void setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value) final;
    virtual quint16 registerValue(QModbusDataUnit::RegisterType type, quint16 address) final;
    virtual void exportRegisterData() final;
    virtual void importRegisterData() final;
protected:
    SAKModbusCommonServerSection *mServerSection;
protected:
    virtual QWidget *bottomSection() final;
};

#endif // SAKMODBUSSERVERCONTROLLERSERVER_HH
