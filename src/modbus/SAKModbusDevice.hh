/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSDEVICE_HH
#define SAKMODBUSDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QLoggingCategory>

class SAKModbusDevice : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName
                   NOTIFY portNameChanged FINAL)
    Q_PROPERTY(int parity READ parity WRITE setParity
                   NOTIFY parityChanged FINAL)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate
                   NOTIFY baudRateChanged FINAL)
    Q_PROPERTY(int dataBits READ dataBits WRITE setDataBits
                   NOTIFY dataBitsChanged FINAL)
    Q_PROPERTY(int stopBits READ stopBits WRITE setStopBits
                   NOTIFY stopBitsChanged FINAL)

    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged FINAL)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged FINAL)
public:
    explicit SAKModbusDevice(const char *lc, QObject *parent = Q_NULLPTR);
    ~SAKModbusDevice();

signals:
    void errorOccurred(const QString &errStr);

protected:
    struct ConnectionParameters {
        struct {
            QString portName;
            int parity;
            int baudRate;
            int dataBits;
            int stopBits;
        } serialport;
        struct {
            QString ip;
            int port;
        } network;
    } mConnectParameters;
    QMutex mConnectParametersMutex;

protected:
    void outputLog(const QString &log, int type);

private:
    QLoggingCategory *mLoggingCategory;

//------------------------------------------------------------------------------
public:
    QString portName();
    void setPortName(QString name);
    int parity();
    void setParity(int parity);
    int baudRate();
    void setBaudRate(int baudRate);
    int dataBits();
    void setDataBits(int bits);
    int stopBits();
    void setStopBits(int bits);
    QString ip();
    void setIp(QString ip);
    int port();
    void setPort(int port);

signals:
    void portNameChanged();
    void parityChanged();
    void baudRateChanged();
    void dataBitsChanged();
    void stopBitsChanged();
    void ipChanged();
    void portChanged();
};

#endif // SAKMODBUSDEVICE_HH
