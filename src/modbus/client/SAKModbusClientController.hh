/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCLIENTCONTROLLER_HH
#define SAKMODBUSCLIENTCONTROLLER_HH

#include <QModbusClient>
#include <QModbusServer>

#include "SAKModbusCommonController.hh"

class SAKModbusCommonClientSection;
class SAKModbusClientController : public SAKModbusCommonController
{
    Q_OBJECT
public:
    SAKModbusClientController(QWidget *parent = Q_NULLPTR);
    virtual void setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value) final;
    virtual quint16 registerValue(QModbusDataUnit::RegisterType type, quint16 address) final;
    virtual void exportRegisterData() final;
    virtual void importRegisterData() final;

    bool tempData(QModbusDataUnit::RegisterType table, quint16 address, quint16 *data);
protected:
    SAKModbusCommonClientSection *mClientSection;
    // The value is for saving data only.
    QModbusServer *mModbusServer;
protected:
    virtual QWidget *bottomSection() final;
    void sendReadRequest(QModbusDataUnit mdu);
    void sendWriteRequest(QModbusDataUnit mdu);
private:
    void readReply();
};

#endif // SAKMODBUSCLIENTCONTROLLER_HH
