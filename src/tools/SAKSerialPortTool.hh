/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDSERIALPORTTOOL_HPP
#define EDSERIALPORTTOOL_HPP

#include <QSerialPort>
#include <QSerialPortInfo>

#include "EDBaseTool.hpp"

class EDSerialPortTool : public EDBaseTool
{
    Q_OBJECT
public:
    EDSerialPortTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setPortName(const QString &portName);
    Q_INVOKABLE void setBaudRate(int baudRate);
    Q_INVOKABLE void setDataBits(int dataBits);
    Q_INVOKABLE void setStopBits(int stopBits);
    Q_INVOKABLE void setParity(int parity);
    Q_INVOKABLE void setFlowControl(int flowControl);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void outputBytesHandler() final;
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

#endif // EDSERIALPORTTOOL_HH
