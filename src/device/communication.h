/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QThread>
#include <QVariantMap>

#include "devicepage/common/abstractio.h"

class Communication : public AbstractIO
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    ~Communication() override;

    Q_INVOKABLE void openDevice();
    Q_INVOKABLE void closeDevice();

    void inputBytes(const QByteArray &bytes);

    virtual QObject *initDevice() { return nullptr; };
    virtual void deinitDevice() {};
    virtual void writeBytes(const QByteArray &bytes) { Q_UNUSED(bytes); };

signals:
    void opened();
    void closed();

    void bytesRead(const QByteArray &bytes, const QString &from);
    void bytesWritten(const QByteArray &bytes, const QString &to);

protected:
    void run() override;

private:
    Q_SIGNAL void invokeWriteBytes(const QByteArray &bytes);
};
