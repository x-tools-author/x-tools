/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUSCLIENT_HH
#define SAKMODBUSCLIENT_HH

#include <QModbusClient>
#include <QModbusRequest>
#include <QModbusDataUnit>
#include "SAKModbusDevice.hh"

class SAKModbusClient : public SAKModbusDevice
{
    Q_OBJECT
    Q_PROPERTY(int numberOfRetries READ numberOfRetries WRITE setNumberOfRetries
                   NOTIFY numberOfRetriesChanged FINAL)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout
                   NOTIFY timeoutChanged FINAL)
public:
    explicit SAKModbusClient(const char *lc, QObject *parent = Q_NULLPTR);

    void sendRawRequest(const QModbusRequest &request, int serverAddress);
    void sendReadRequest(const QModbusDataUnit &read, int serverAddress);
    void sendReadWriteRequest(const QModbusDataUnit &read,
                              const QModbusDataUnit &write, int serverAddress);
    void sendWriteRequest(const QModbusDataUnit &write, int serverAddress);

signals:
    void errorOccurred(const QString &errStr);
    void stateChanged(int state);

protected:
    virtual void run() final;
    // You should create a client and initialize it in this interface.
    virtual QModbusClient *createClient() = 0;

private:
    QModbusClient *mClient{Q_NULLPTR};

private:
    Q_SIGNAL void invokeSendRawRequest(const QModbusRequest &request,
                                       int serverAddress);
    Q_SIGNAL void invokeSendReadRequest(const QModbusDataUnit &read,
                                        int serverAddress);
    Q_SIGNAL void invokeSendReadWriteRequest(const QModbusDataUnit &read,
                                    const QModbusDataUnit &write,
                                    int serverAddress);
    Q_SIGNAL void invokeSendWriteRequest(const QModbusDataUnit &write,
                                int serverAddress);

private:
    void onInvokeSendRawRequest(const QModbusRequest &request,
                                int serverAddress);
    void onInvokeSendReadRequest(const QModbusDataUnit &read,
                                 int serverAddress);
    void onInvokeSendReadWriteRequest(const QModbusDataUnit &read,
                                      const QModbusDataUnit &write,
                                      int serverAddress);
    void onInvokeSendWriteRequest(const QModbusDataUnit &write,
                                  int serverAddress);

//------------------------------------------------------------------------------
private:
    struct ClientParamters {
        int numberOfRetries;
        int timeout;
    } mClientParameters;
    QMutex mClientParametersMutex;

public:
    int numberOfRetries();
    void setNumberOfRetries(int numberOfRetries);

    int timeout();
    void setTimeout(int timeout);

signals:
    void numberOfRetriesChanged();
    void timeoutChanged();
};

#endif // SAKMODBUSCLIENT_HH
