/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QThread>
#include <QVariant>

namespace xTools {

class BleScanner : public QThread
{
    Q_OBJECT
public:
    explicit BleScanner(QObject *parent = nullptr);
    ~BleScanner();

signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void errorOccurred(const QString &errStr);

protected:
    virtual void run() override;

private:
    QBluetoothDeviceDiscoveryAgent *m_discover;

private:
    void onDiscoveryFinished();
    void onDiscoveryErrorOccurred();
    void onDiscoveryDeviceDiscovered(const QBluetoothDeviceInfo &info);
};

} // namespace xTools
