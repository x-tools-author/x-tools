/*
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTDEVICE_HH
#define SAKTCPCLIENTDEVICE_HH

#include <QThread>
#include <QTcpSocket>

#include "SAKDebuggerDevice.hh"

class SAKTcpClientDevice:public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKTcpClientDevice(QSettings *settings,
                       const QString &settingsGroup,
                       QWidget *uiParent = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);
protected:
    bool initialize() final;
    ReadContextVector read() final;
    WriteContext write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QString mLocalHost;
    quint16 mLocalPort;
    bool mSpecifyClientAddressAndPort;
    QString mServerHost;
    quint16 mServerPort;
    QTcpSocket *mTcpSocket;
signals:
    void serverInfoChanged(QString info);
};

#endif
