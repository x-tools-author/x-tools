/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGGERDEVICESESERIALPORT_HH
#define SAKDEBUGGERDEVICESESERIALPORT_HH

#include <QSerialPort>
#include "SAKDebuggerDevice.hh"

class SAKDebugger;
class SAKDebuggerDeviceSerialport : public SAKDebuggerDevice
{
    Q_OBJECT
    Q_PROPERTY(QStringList serialports READ serialports NOTIFY serialportsChanged)
    Q_PROPERTY(QStringList baudRates READ baudRates CONSTANT)
    Q_PROPERTY(QStringList stopBits READ stopBits CONSTANT)
    Q_PROPERTY(QStringList paritys READ paritys CONSTANT)
    Q_PROPERTY(QStringList dataBits READ dataBits CONSTANT)
public:
    SAKDebuggerDeviceSerialport(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDeviceSerialport();

    static QStringList avalidSerialports();
    static QStringList avalidBaudRates();
    static QStringList avalidStopBits();
    static QStringList avalidParitys();
    static QStringList avalidDataBits();
protected:
    void run() final;
private:
    QSerialPort *serialport;
private:
    QStringList _serialports;
    QStringList serialports();

    QStringList _baudRates;
    QStringList baudRates();

    QStringList _stopBits;
    QStringList stopBits();

    QStringList _paritys;
    QStringList paritys();

    QStringList _dataBits;
    QStringList dataBits();
signals:
    void serialportsChanged();
};

#endif
