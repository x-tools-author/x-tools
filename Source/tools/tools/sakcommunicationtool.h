/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKCOMMUNICATIONTOOL_H
#define SAKCOMMUNICATIONTOOL_H

#include "sakbasetool.h"

#include <QMutex>

class SAKCommunicationTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKCommunicationTool(QObject *parent = nullptr);
    ~SAKCommunicationTool() override;
    Q_INVOKABLE void inputBytes(const QByteArray &bytes) override;

signals:
    void bytesRead(const QByteArray &bytes, const QString &from);
    void bytesWritten(const QByteArray &bytes, const QString &to);

protected:
    void run() override;

    virtual bool initialize(QString &errStr) = 0;
    virtual void writeBytes(const QByteArray &bytes) = 0;
    virtual void uninitialize() = 0;

private:
    QList<QByteArray> m_inputBytesList;
    QMutex m_inputBytesMutex;
};

#endif // SAKCOMMUNICATIONTOOL_H
