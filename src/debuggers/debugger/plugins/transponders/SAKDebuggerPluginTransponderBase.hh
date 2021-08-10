/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTRANSPONDERBASE_HH
#define SAKDEBUGGERPLUGINTRANSPONDERBASE_HH

#include <QMutex>
#include <QWidget>
#include <QThread>

class SAKDebuggerPluginTransponderBase : public QThread
{
    Q_OBJECT
public:
    SAKDebuggerPluginTransponderBase(QObject *parent);
    ~SAKDebuggerPluginTransponderBase();

    void writeBytes(QByteArray bytes);


protected:
    struct SAKStructTransponderProtectedSignal {};


protected:
    QByteArray takeBytes();
    void run() final;

    virtual bool initialize() = 0;
    virtual QByteArray read() = 0;
    virtual QByteArray write(const QByteArray &bytes) = 0;
    virtual void uninitialize() = 0;
    virtual QWidget *controller() = 0;


private:
    QVector<QByteArray> mBytesVector;
    QMutex mBytesVectorMutex;


signals:
    void bytesWritten(QByteArray bytes);
    void bytesRead(QByteArray bytes);
    void errorOccurred(QString error);


    void readyRead(
            SAKDebuggerPluginTransponderBase::SAKStructTransponderProtectedSignal
            );
};

#endif
