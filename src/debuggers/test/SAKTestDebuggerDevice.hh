/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKTESTDEBUGGERDEVICE_HH
#define SAKTESTDEBUGGERDEVICE_HH
#include <QMutex>
#include "SAKDebuggerDevice.hh"
#include "SAKTestDebuggerController.hh"


class SAKTestDebuggerDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKTestDebuggerDevice(QSettings *settings,
                          const QString &settingsGroup,
                          QWidget *uiParent = Q_NULLPTR,
                          QObject *parent = Q_NULLPTR);
    ~SAKTestDebuggerDevice();

    void generateReadData(bool start, int interval);
    void generateWriteData(bool start, int interval);
protected:
    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;
protected:
    void timerEvent(QTimerEvent *event) final;
private:    
    int mReadDataTimerId;
    int mWriteDateTimerId;
};

#endif
