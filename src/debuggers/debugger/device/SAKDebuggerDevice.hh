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
#include "SAKDebuggerDeviceAnalyzer.hh"

/// @brief device abstract class
class SAKDebuggerDevice:public QThread
{
    Q_OBJECT
public:
    SAKDebuggerDevice(QSettings *settings,
                      const QString &settingsGroup,
                      QWidget *uiParent = Q_NULLPTR,
                      QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDevice();

    void writeBytes(QByteArray bytes);
    void setupMenu(QMenu *menu);
    QVariant parametersContext();
    void setParametersContext(QVariant parametersContext);
protected:
    struct SAKDeviceProtectedSignal {};
    struct SAKStructDeviceContext {
        QByteArray bytes;
        QString flag = "";
    };
    typedef SAKStructDeviceContext ReadContext;
    typedef SAKStructDeviceContext WriteContext;
    typedef QVector<ReadContext> ReadContextVector;
    typedef QVector<WriteContext> WriteContextsVector;

protected:
    QByteArray takeBytes();
    void run() override;

    virtual bool initialize() = 0;
    virtual ReadContextVector read() = 0;
    virtual WriteContext write(const QByteArray &bytes) = 0;
    virtual void uninitialize() = 0;
signals:
    void readyRead(SAKDebuggerDevice::SAKDeviceProtectedSignal);
private:
    struct SAKStructDevicePatametersContext {
        SAKDebuggerDeviceMask::SAKStructMaskContext maskCtx ;
        SAKDebuggerDeviceAnalyzer::SAKStructAnalyzerContext analyzerCtx;
    }mInnerParametersContext;
    struct SAKStructDeviceAnalyzerContext {
        QByteArray bytesTemp;
        const int maxTempLangth = 2048;
    }mAnalyzerCtx;
private:
    QSettings *settings;
    const QString settingsGroup;
    QMutex mInnerParametersContextMutex;
    QMutex mParametersContextMutex;
    QVariant mParametersContext;
    QVector<QByteArray> mBytesVector;
    QMutex mBytesVectorMutex;
    QMutex mAnalyzerCtxMutex;
    // Parameters editors
    SAKDebuggerDeviceMask *mMask;
    SAKDebuggerDeviceAnalyzer *mAnalyzer;
private:
    QByteArray mask(const QByteArray &plaintext, bool isRxData);
    void analyzer(QByteArray data);
    SAKStructDevicePatametersContext innerParametersContext();
signals:
    void bytesWritten(QByteArray bytes, const QString &to);
    void bytesRead(QByteArray bytes, const QString &from);
    void errorOccurred(QString error);
    void invokeOutputMessage(const QString &info);
};

#endif
