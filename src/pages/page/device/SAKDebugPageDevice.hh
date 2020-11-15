/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGDEVICE_HH
#define SAKDEBUGPAGDEVICE_HH
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/// @brief device abstract class
class SAKDebugPage;
class SAKDebugPageDeviceMask;
class SAKDebugPageDevice:public QThread
{
    Q_OBJECT
public:
    SAKDebugPageDevice(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageDevice();

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
    SAKDebugPage *mDebugPage;
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
    QMutex mWaitingForWritingBytesListMutex;
    QList<QByteArray> mWaitingForWritingBytesList;
    QList<SettingsPanel> mSettingsPanelList;
signals:
    void bytesWritten(QByteArray bytes);
    void bytesRead(QByteArray bytes);
    void messageChanged(QString msg, bool isInfo);
    void deviceStateChanged(bool isOpened);
};

#endif
