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

class Device : public QThread
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    ~Device() override;

    void openDevice();
    void closeDevice();

    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &data);
    virtual QObject *initDevice() { return nullptr; };
    virtual void deinitDevice() {};
    virtual void writeBytes(const QByteArray &bytes) { Q_UNUSED(bytes); };

signals:
    void opened();
    void closed();

    void bytesRead(const QByteArray &bytes, const QString &from);
    void bytesWritten(const QByteArray &bytes, const QString &to);

    void warningOccurred(const QString &warningString);
    void errorOccurred(const QString &errorString);

protected:
    void run() override;

private:
    Q_SIGNAL void invokeWriteBytes(const QByteArray &bytes);

private:
    QVariantMap m_parameters;
};
