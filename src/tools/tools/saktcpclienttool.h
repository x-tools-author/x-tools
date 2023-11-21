/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTCPCLIENTTOOL_H
#define SAKTCPCLIENTTOOL_H

#include "saksocketclienttool.h"
#include <QTcpSocket>

class SAKTcpClientTool : public SAKSocketClientTool
{
    Q_OBJECT
public:
    explicit SAKTcpClientTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes, const QVariant &context = QJsonObject()) final;
    void readBytes();
    virtual void uninitialize() final;

private:
    QTcpSocket *mTcpSocket{nullptr};
};

#endif // SAKTCPCLIENTTOOL_H
