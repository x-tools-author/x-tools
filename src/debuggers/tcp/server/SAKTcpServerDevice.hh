/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERDEVICE_HH
#define SAKTCPSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "SAKDebuggerDevice.hh"


class SAKTcpServerDevice:public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKTcpServerDevice(QSettings *settings,
                       const QString &settingsGroup,
                       QWidget *uiParent = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);
protected:
    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QTcpServer *mTcpServer;
    QList<QTcpSocket*> mClientList;
signals:
    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
};

#endif
