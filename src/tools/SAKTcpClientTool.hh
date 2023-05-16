/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPCLIENTTOOL_HH
#define SAKTCPCLIENTTOOL_HH

#include <QTcpSocket>
#include "SAKSocketClientTool.hh"

class SAKTcpClientTool : public SAKSocketClientTool
{
    Q_OBJECT
public:
    explicit SAKTcpClientTool(QObject *parent = nullptr);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    void readBytes();
    virtual void uninitialize() final;

private:
    QTcpSocket *mTcpSocket{nullptr};
};

#endif // SAKTCPCLIENTTOOL_H
