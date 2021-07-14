/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTESTDEVICE_HH
#define SAKTESTDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKDebugPageDevice.hh"

class SAKTestDebugPage;
class SAKTestDeviceController;
/// @brief The class is used by developer to debug
class SAKTestDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKTestDevice(SAKTestDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKTestDevice();
protected:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    QByteArray writeForTest() final;
    void close() final;
    void free() final;
private:
    SAKTestDebugPage *mDebugPage;
    SAKTestDeviceController *mController;
    qint64 mOldReadTimestamp;
    qint64 mNewReadTimestamp;
    qint64 mOldWrittingTimestamp;
    qint64 mNewWrittingTimestamp;
};

#endif
