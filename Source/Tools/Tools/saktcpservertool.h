/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
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
