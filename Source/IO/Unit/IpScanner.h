/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>
#include <QTimer>

namespace xTools {

class IpScanner : public QObject
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(QStringList ipList READ ipList NOTIFY ipListChanged FINAL)
    Q_PROPERTY(bool enableIpV4 READ enableIpV4 WRITE setEnableIpV4 NOTIFY enableIpV4Changed FINAL)
    Q_PROPERTY(bool enableIpV6 READ enableIpV6 WRITE setEnableIpV6 NOTIFY enableIpV6Changed FINAL)
    Q_PROPERTY(bool enableAutoRefresh READ enableAutoRefresh WRITE setEnableAutoRefresh NOTIFY enableAutoRefreshChanged FINAL)
    // clang-format on
public:
    explicit IpScanner(QObject *parent = nullptr);

private:
    QTimer *m_refreshTimer;

private:
    void refresh();

private:
    QStringList m_ipList;
    QStringList ipList();
    Q_SIGNAL void ipListChanged();

    bool m_enableIpV4{true};
    bool enableIpV4();
    void setEnableIpV4(bool enable);
    Q_SIGNAL void enableIpV4Changed();

    bool m_enableIpV6{false};
    bool enableIpV6();
    void setEnableIpV6(bool enable);
    Q_SIGNAL void enableIpV6Changed();

    bool m_enableAutoRefresh{true};
    bool enableAutoRefresh();
    void setEnableAutoRefresh(bool enable);
    Q_SIGNAL void enableAutoRefreshChanged();
};

} // namespace xTools
