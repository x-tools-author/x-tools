/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEDEVICE_HH
#define SAKDDEBUGPAGEEVICE_HH
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/// @brief device abstract class
class SAKDebugPageDevice:public QThread
{
    Q_OBJECT
public:
    SAKDebugPageDevice(QObject *parent = Q_NULLPTR);

    /**
     * @brief wakeMe: wake the thread
     */
    void wakeMe();

    /**
     * @brief writeBytes: write bytes to device
     * @param bytes: bytes need to be wirtten
     */
    void writeBytes(QByteArray bytes);
protected:
    QMutex mThreadMutex;
    QWaitCondition mThreadWaitCondition;
protected:
    QByteArray takeWaitingForWrittingBytes();
private:
    QMutex mWaitingForWritingBytesListMutex;
    QList<QByteArray> mWaitingForWritingBytesList;
signals:
    void bytesWritten(QByteArray bytes);
    void bytesRead(QByteArray bytes);
    void messageChanged(QString msg, bool isInfo);
    void deviceStateChanged(bool isOpened);
};

#endif
