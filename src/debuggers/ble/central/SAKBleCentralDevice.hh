/******************************************************************************
 *  Copyright 2022 wuhai(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALDEVICET_HH
#define SAKBLECENTRALDEVICET_HH

#include <QMutex>
#include <QWaitCondition>
#include <QLowEnergyController>

#include "SAKDebuggerDevice.hh"

class SAKBleCentralDebugger;
class SAKBleCentralController;
class SAKBleCentralDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKBleCentralDevice(QSettings *settings,
                        const QString &settingsGroup,
                        QWidget *uiParent,
                        QObject *parent = Q_NULLPTR);
signals:
    void servicesChanged(const QStringList services);
protected:
    bool initialize() final;
    ReadContextVector read() final;
    WriteContext write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QLowEnergyController *mBleController;
};

#endif
