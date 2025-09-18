/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QMutex>
#include <QSerialPortInfo>
#include <QThread>
#include <QVariantList>

class SerialPortScanner : public QThread
{
    Q_OBJECT
public:
    explicit SerialPortScanner(QObject *parent = nullptr);
    ~SerialPortScanner() override;

    Q_INVOKABLE void setIsBusyDevicesIgnored(bool ignored);
    Q_INVOKABLE QStringList baudRates() const;
    QStringList portNames();
    QList<QSerialPortInfo> deviceInfos();

signals:
    void portNamesChanged(const QStringList &portNames);
    void devicesChanged(const QList<QSerialPortInfo> &devices);

protected:
    void run() override;

private:
    QStringList refresh();
    bool isBusyDevice(const QString &portName);

private:
    std::atomic_bool m_isBusyDevicesIgnored{false};
    QList<QSerialPortInfo> m_lastDevices;
};
