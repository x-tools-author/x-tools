/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKUDPSERVERTOOL_H
#define SAKUDPSERVERTOOL_H

#include "saksocketservertool.h"
#include <QUdpSocket>

class SAKUdpServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKUdpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) override;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() final;
    void readBytes();

private:
    QUdpSocket *mUdpSocket{nullptr};

private:
    void writeDatagram(const QByteArray &bytes, const QString &ip, quint16 port);
};

#endif // SAKUDPSERVERTOOL_H
