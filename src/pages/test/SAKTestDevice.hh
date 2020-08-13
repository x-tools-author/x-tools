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
class SAKTestDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKTestDevice(SAKTestDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKTestDevice();
protected:
    void run() final;
private:
    SAKTestDebugPage *mDebugPage;
};

#endif
