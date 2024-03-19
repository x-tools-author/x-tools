/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>

#include "xToolsCommunicationTool.h"

class xToolsSerialPortTool : public xToolsCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged FINAL)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged FINAL)
    Q_PROPERTY(int dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged FINAL)
    Q_PROPERTY(int stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged FINAL)
    Q_PROPERTY(int parity READ parity WRITE setParity NOTIFY parityChanged FINAL)
    Q_PROPERTY(int flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged FINAL)

public:
    xToolsSerialPortTool(QObject *parent = Q_NULLPTR);

    QString portName();
    void setPortName(const QString &portName);

    int baudRate();
    void setBaudRate(int baudRate);

    int dataBits();
    void setDataBits(int dataBits);

    int stopBits();
    void setStopBits(int stopBits);

    int parity();
    void setParity(int parity);

    int flowControl();
    void setFlowControl(int flowControl);

signals:
    void portNameChanged();
    void baudRateChanged();
    void dataBitsChanged();
    void stopBitsChanged();
    void parityChanged();
    void flowControlChanged();

protected:
    bool initialize(QString &errStr) override;
    void writeBytes(const QByteArray &bytes) override;
    void uninitialize() override;

    void readBytes();

private:
    struct
    {
        QString portName{""};
        int baudRate{9600};
        int dataBits{QSerialPort::Data8};
        int stopBits{QSerialPort::OneStop};
        int parity{QSerialPort::NoParity};
        int flowControl{QSerialPort::NoFlowControl};
    } m_parameters;

private:
    QSerialPort *m_serialPort{Q_NULLPTR};
};
