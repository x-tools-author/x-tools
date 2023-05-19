/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPSERVERTOOL_HH
#define SAKTCPSERVERTOOL_HH

#include <QTcpServer>
#include "SAKSocketServerTool.hh"

class SAKTcpServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKTcpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void uninitialize() final;

private:
    void writeBytesInner(QTcpSocket *client, const QByteArray &bytes,
                         const QVariant &context = QJsonObject());

private:
    QTcpServer *mTcpServer{nullptr};
    QList<QTcpSocket*> mTcpSocketList;
};

#endif // SAKTCPSERVERTOOL_H
