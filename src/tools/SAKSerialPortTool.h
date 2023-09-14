/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
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

#include "SAKCommunicationTool.h"

class SAKSerialPortTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged FINAL)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged FINAL)
    Q_PROPERTY(int dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged FINAL)
    Q_PROPERTY(int stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged FINAL)
    Q_PROPERTY(int parity READ parity WRITE setParity NOTIFY parityChanged FINAL)
    Q_PROPERTY(int flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged FINAL)

public:
    SAKSerialPortTool(QObject *parent = Q_NULLPTR);

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
    virtual bool initialize(QString &errStr) final;
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
