/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTDEVICE_HH
#define SAKWEBSOCKETCLIENTDEVICE_HH

#include <QMutex>
#include <QWebSocket>
#include <QAbstractSocket>

#include "SAKDebuggerDevice.hh"

class SAKWebSocketClientDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKWebSocketClientDevice(QSettings *settings,
                             const QString &settingsGroup,
                             QWidget *uiParent = Q_NULLPTR,
                             QObject *parent = Q_NULLPTR);
protected:
    bool initialize() final;
    ReadContextVector read() final;
    WriteContext write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QWebSocket *mWebSocket;
    QVector<QByteArray> mByteArrayVector;
    QMutex mByteArrayVectorMutex;
private:
    void appendMessage(const QByteArray &byteArray);
    QByteArray takeMessage();
signals:
    void clientInfoChanged(QString info);
};

#endif
