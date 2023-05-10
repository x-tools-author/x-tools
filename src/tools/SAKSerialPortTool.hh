/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSERIALPORTTOOL_HH
#define SAKSERIALPORTTOOL_HH

#include <QSerialPort>
#include <QSerialPortInfo>

#include "SAKCommunicationTool.hh"

class SAKSerialPortTool : public SAKCommunicationTool
{
    Q_OBJECT
public:
    SAKSerialPortTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setPortName(const QString &portName);
    Q_INVOKABLE void setBaudRate(int baudRate);
    Q_INVOKABLE void setDataBits(int dataBits);
    Q_INVOKABLE void setStopBits(int stopBits);
    Q_INVOKABLE void setParity(int parity);
    Q_INVOKABLE void setFlowControl(int flowControl);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void readBytes() final;
    virtual void uninitialize() final;

private:
    struct {
        QString portName{""};
        int baudRate{9600};
        int dataBits{QSerialPort::Data8};
        int stopBits{QSerialPort::OneStop};
        int parity{QSerialPort::NoParity};
        int flowControl{QSerialPort::NoFlowControl};
    } mParameters;

private:
    QSerialPort *mSerialPort{Q_NULLPTR};
};

#endif // SAKSERIALPORTTOOL_H
