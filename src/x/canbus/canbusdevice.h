/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QJsonObject>
#include <QMutex>
#include <QPair>
#include <QThread>
#include <QVariant>

namespace xCanBus {

struct CanBusDeviceParameters
{
    QString plugin;
    QString interfaceName;
    QList<QPair<QCanBusDevice::ConfigurationKey, QVariant>> params;
};

class CanBusDevice : public QThread
{
    Q_OBJECT
public:
    explicit CanBusDevice(QObject* parent = nullptr);
    ~CanBusDevice() override;

    void startDevice(const CanBusDeviceParameters& params);
    void stopDevice();
    void writeFrame(const QCanBusFrame& frame);

Q_SIGNALS:
    void opened();
    void errorOccurred(const QString& errorString);
    void frameRx(const QCanBusFrame& frame);
    void frameTx(const QCanBusFrame& frame);

protected:
    void run() override;

private:
    CanBusDeviceParameters m_parameters;
    QMutex m_parametersMutex;

    Q_SIGNAL void invokeWriteFrame(const QCanBusFrame& frame);
};

} // namespace xCanBus