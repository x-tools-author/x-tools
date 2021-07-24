/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERDEVICE_HH
#define SAKDEBUGGERDEVICE_HH
#include <QMenu>
#include <QMutex>
#include <QThread>
#include <QSettings>
#include <QWaitCondition>

#include "SAKDebuggerDeviceMask.hh"

/// @brief device abstract class
class SAKDebuggerDevice:public QThread
{
    Q_OBJECT
public:
    SAKDebuggerDevice(QSettings *settings,
                      const QString &settingsGroup,
                      QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDevice();

    void writeBytes(QByteArray bytes);
    void setupMenu(QMenu *menu);


protected:
    struct SAKDeviceProtectedSignal {};
protected:
    QByteArray takeBytes();
    void run() override;

    virtual bool initialize() = 0;
    virtual QByteArray read() = 0;
    virtual QByteArray write(const QByteArray &bytes) = 0;
    virtual void uninitialize() = 0;
signals:
    void readyRead(SAKDebuggerDevice::SAKDeviceProtectedSignal);


private:
    struct SAKStructDevicePatametersContext {
        SAKDebuggerDeviceMask::SAKStructMaskContext maskCtx ;
        struct AnalyzerContext {
            bool enable;
            bool fixedLength;
            int length;
            QByteArray startFlags;
            QByteArray endFlags;
        } analyzerCtx;
    };


private:
    QSettings *settings;
    const QString settingsGroup;
    SAKStructDevicePatametersContext mParametersCtx;
    QMutex mParametersCtxMutex;
    QVector<QByteArray> mBytesVector;
    QMutex mBytesVectorMutex;
    // Parameters editors
    SAKDebuggerDeviceMask *mMask;


private:
    QByteArray mask(const QByteArray &plaintext, bool isRxData);
    SAKStructDevicePatametersContext parametersContext();


signals:
    void bytesWritten(QByteArray bytes);
    void bytesRead(QByteArray bytes);
    void errorOccurred(QString error);
};

#endif
