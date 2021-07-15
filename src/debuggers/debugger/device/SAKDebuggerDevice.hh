/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGGERDEVICE_HH
#define SAKDEBUGGERDEVICE_HH
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/// @brief device abstract class
class SAKDebugger;
class SAKDebugPageDeviceMask;
class SAKDebuggerDevice:public QThread
{
    Q_OBJECT
public:
    SAKDebuggerDevice(SAKDebugger *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDevice();

    /**
     * @brief wakeMe: wake the thread
     */
    void requestWakeup();

    /**
     * @brief writeBytes: write bytes to device
     * @param bytes: bytes need to be wirtten
     */
    void writeBytes(QByteArray bytes);

    struct SettingsPanel{
        QString name;
        QWidget *panel;
    };
    QList<SettingsPanel> settingsPanelList();
protected:
    QMutex mThreadMutex;
    QWaitCondition mThreadWaitCondition;
    SAKDebugger *mDebugPage;
    SAKDebugPageDeviceMask *mDeviceMask;
protected:
    QByteArray takeWaitingForWrittingBytes();
    void run() override;

    // Realize these functions in the subclass only, do not call them.
    virtual bool initializing(QString &errorString);
    virtual bool open(QString &errorString);
    virtual QByteArray read();
    virtual QByteArray write(QByteArray bytes);
    virtual bool checkSomething(QString &errorString);
    virtual void close();
    virtual void free();

    /**
     * @brief writeForTest: The interface is just for debugging data stream, do not override the interface
     * @return Test data
     */
    virtual QByteArray writeForTest();
private:
    struct SAKStructDevicePatametersContext {
        struct MaskContext {
            quint8 rx;
            quint8 tx;
            bool enableRx;
            bool enableTx;
        } maskCtx;
        struct AnalyzerContext {
            bool enable;
            bool fixedLength;
            int length;
            QByteArray startFlags;
            QByteArray endFlags;
        } analyzerCtx;
    } m_parametersCtx;

    QMutex m_parametersCtxMutex;
    QMutex mWaitingForWritingBytesListMutex;
    QList<QByteArray> mWaitingForWritingBytesList;
    QList<SettingsPanel> mSettingsPanelList;
private:
    QByteArray mask(const QByteArray &plaintext, bool isRxData);
    SAKStructDevicePatametersContext parametersContext();
signals:
    void bytesWritten(QByteArray bytes);
    void bytesRead(QByteArray bytes);
    void messageChanged(QString msg, bool isInfo);
    void deviceStateChanged(bool isOpened);
};

#endif
