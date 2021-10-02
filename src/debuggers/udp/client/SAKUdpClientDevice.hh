/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEVICE_HH
#define SAKUDPCLIENTDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QUdpSocket>

#include "SAKDebuggerDevice.hh"
#include "SAKCommonDataStructure.hh"

class SAKUdpClientDebugger;
class SAKUdpClientController;
class SAKUdpClientDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKUdpClientDevice(QSettings *settings,
                       const QString &settingsGroup,
                       QWidget *uiParent = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);
    ~SAKUdpClientDevice();

    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QUdpSocket *mUdpSocket;
signals:
    void clientInfoChanged(QString info);
};
#endif
