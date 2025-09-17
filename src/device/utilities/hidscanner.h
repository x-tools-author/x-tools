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

#include <QList>
#include <QMutex>
#include <QThread>

struct HidDeviceInfo
{
    QString path;
    unsigned short vendorId;
    unsigned short productId;
    QString serialNumber;
    unsigned short releaseNumber;
    QString manufacturerString;
    QString productString;
    unsigned short usagePage;
    unsigned short usage;
    int interfaceNumber;
    int busType;
};

class HidScanner : public QThread
{
    Q_OBJECT
public:
    explicit HidScanner(QObject *parent = nullptr);
    ~HidScanner() override;

    Q_INVOKABLE void setIsBusyDevicesIgnored(bool ignored);
    Q_INVOKABLE QList<HidDeviceInfo> hidInfos();

signals:
    void devicesChanged(const QList<HidDeviceInfo> &infos);

protected:
    void run() override;

private:
    QList<HidDeviceInfo> refresh();
    bool isBusyDevice(const QString &portName);

private:
    std::atomic_bool m_isBusyDevicesIgnored{false};
    QList<HidDeviceInfo> m_lastInfos;
};

Q_DECLARE_METATYPE(HidDeviceInfo)