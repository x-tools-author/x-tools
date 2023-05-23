/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKDEVICE_HH
#define SAKDEVICE_HH

#include <QMutex>
#include <QTimer>
#include <QThread>
#include "SAKObj.hh"

class SAKDevice : public QThread, public SAKObj
{
    Q_OBJECT
public:
    explicit SAKDevice(QObject *parent = Q_NULLPTR);
    void writeBytes(const QByteArray &bytes);
signals:
    void bytesRead(const QByteArray &byte, const QString &from = "");
    void bytesWritten(const QByteArray &byte, const QString &to = "");
public:
    enum class SAKEnumResponseType {
        Equal,
        DoNotEqual,
        Contain,
        DoNotContain,
        AlwaysAllow
    };

    struct SAKStructResponseItemContext {

    };

    struct SAKStructDeviceParameterContext {
        bool timingReading;
        struct {
            bool enableTx;
            bool enableRx;
            quint8 tx;
            quint8 rx;
        } masker;
        struct {
            bool enableResponseAutomatically;

        };
    };
protected:
    struct SAKDeviceProtectedSignal {};
protected:
    virtual bool initDevice() = 0;
    virtual bool uninitDevice() = 0;

    virtual QByteArray readBytesActually() = 0;
    virtual bool writeBytesActually(const QByteArray &bytes) = 0;
protected:
    void run() override;
    QByteArray takeBytes();
    void writeBytesVirtual();
signals:
    void readyRead(SAKDevice::SAKDeviceProtectedSignal);
private:
    QMutex mBytesVectorMutex;
    QVector<QByteArray> mBytesVector;
    SAKStructDeviceParameterContext mParameterCtx;
private:
    void initRx(QTimer &rxTimer);
    void initTx(QTimer &txTimer);
};

#endif // SAKDEVICE_HH
