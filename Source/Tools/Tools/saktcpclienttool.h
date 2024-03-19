/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
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
    virtual void writeBytes(const QByteArray &bytes) final;
    void readBytes();
    virtual void uninitialize() final;

private:
    QTcpSocket *mTcpSocket{nullptr};
};

#endif // SAKTCPCLIENTTOOL_H
