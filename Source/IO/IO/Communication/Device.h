/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
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

#include "../AbstractIO.h"

class Device : public AbstractIO
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    ~Device();

    void openDevice();
    void closeDevice();
    void writeBytes(const QByteArray &bytes);

    virtual void setParameters(const QVariantMap &parameters);
    virtual QObject *initDevice() { return nullptr; };
    virtual void deinitDevice() {};
    virtual void writeBytesToDevice(const QByteArray &bytes) {};

signals:
    void opened();
    void closed();

    void bytesRead(const QByteArray &bytes, const QString &from);
    void bytesWritten(const QByteArray &bytes, const QString &to);

protected:
    QVariantMap m_parameters;
    QMutex m_parametersMutex;

protected:
    void run() override;

private:
    QObject *m_deviceObj{nullptr};

private:
    Q_SIGNAL void invokeWriteBytes(const QByteArray &bytes);
};
