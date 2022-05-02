/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QMutex>
#include <QSerialPort>
#include <QWaitCondition>

#include "SAKDebuggerDevice.hh"
#include "SAKCommonDataStructure.hh"

class SAKSerialPortDebugger;
class SAKSerialPortController;
class SAKSerialPortDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKSerialPortDevice(QSettings *settings,
                        const QString &settingsGroup,
                        QWidget *uiParent,
                        QObject *parent = Q_NULLPTR);
protected:
    bool initialize() final;
    ReadContextVector read() final;
    WriteContext write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QSerialPort *mSerialPort;
};

#endif
