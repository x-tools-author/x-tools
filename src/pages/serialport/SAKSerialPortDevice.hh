/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QMutex>
#include <QSerialPort>
#include <QWaitCondition>

#include "SAKDebugPageDevice.hh"

class SAKSerialPortDebugPage;
class SAKSerialPortDeviceController;
class SAKSerialPortDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKSerialPortDevice(SAKSerialPortDebugPage *debugPage, QObject *parent = Q_NULLPTR);

    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    void close() final;
    void free() final;
private:
    QSerialPort *mSerialPort;
    SAKSerialPortDebugPage *mDebugPage;
};

#endif
