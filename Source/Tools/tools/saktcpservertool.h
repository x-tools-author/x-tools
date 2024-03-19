/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTCPSERVERTOOL_H
#define SAKTCPSERVERTOOL_H

#include "saksocketservertool.h"
#include <QMutex>
#include <QTcpServer>

class SAKTcpServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKTcpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) override;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() final;

private:
    void writeBytesInner(QTcpSocket *client, const QByteArray &bytes);

private:
    QTcpServer *m_tcpServer{nullptr};
    QList<QTcpSocket *> m_tcpSocketList;
    QMutex m_tcpSocketListMutex;
};

#endif // SAKTCPSERVERTOOL_H
