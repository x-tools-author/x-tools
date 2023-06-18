/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPSERVERTOOL_H
#define SAKUDPSERVERTOOL_H

#include <QUdpSocket>
#include "SAKSocketServerTool.hh"

class SAKUdpServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKUdpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void uninitialize() final;
    void readBytes();

private:
    QUdpSocket *mUdpSocket{nullptr};

private:
    void writeDatagram(const QByteArray &bytes, const QVariant &context,
                       const QString &ip, quint16 port);
};

#endif // SAKUDPSERVERTOOL_H
