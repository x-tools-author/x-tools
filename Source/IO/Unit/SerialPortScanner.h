/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QMutex>
#include <QThread>

namespace xTools {

class SerialPortScanner : public QThread
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(QStringList portNames READ portNames NOTIFY portNamesChanged)
    Q_PROPERTY(QStringList baudRates READ baudRates NOTIFY baudRatesChanged)
    Q_PROPERTY(bool isBusyDevicesIgnored READ isBusyDevicesIgnored WRITE setIsBusyDevicesIgnored NOTIFY isBusyDevicesIgnoredChanged FINAL)
    Q_PROPERTY(bool isHotSwapEnabled READ isHotSwapEnabled WRITE setIsHotSwapEnabled NOTIFY isHotSwapEnabledChanged FINAL)
    Q_PROPERTY(bool isBusy READ isBusy WRITE setIsBusy NOTIFY isBusyChanged FINAL)
    // clang-format on
public:
    explicit SerialPortScanner(QObject *parent = nullptr);
    ~SerialPortScanner();

protected:
    void run() override;

private:
    void refresh();
    bool isBusyDevice(const QString &portName);

private:
    QMutex m_mutex;

    QStringList m_portNames;
    QStringList portNames();
    Q_SIGNAL void portNamesChanged();

    QStringList m_baudRates;
    QStringList baudRates();
    Q_SIGNAL void baudRatesChanged();

    std::atomic_bool m_isBusyDevicesIgnored{false};
    bool isBusyDevicesIgnored();
    void setIsBusyDevicesIgnored(bool ignored);
    Q_SIGNAL void isBusyDevicesIgnoredChanged();

    std::atomic_bool m_hotSwapEnabled{false};
    bool isHotSwapEnabled();
    void setIsHotSwapEnabled(bool enabled);
    Q_SIGNAL void isHotSwapEnabledChanged();

    std::atomic_bool m_isBusy{false};
    bool isBusy();
    void setIsBusy(bool busy);
    Q_SIGNAL void isBusyChanged();
};

} // namespace xTools
