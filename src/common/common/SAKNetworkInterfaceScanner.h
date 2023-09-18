/******************************************************************************
 * Copyright 2023 wuuhaii(qsaker@foxmail.com). All rights reserved.
 *****************************************************************************/
#ifndef SAKNETWORKINTERFACESCANNER_H
#define SAKNETWORKINTERFACESCANNER_H

#include <QTimer>
#include <QObject>

class SAKNetworkInterfaceScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList ipList READ ipList NOTIFY ipListChanged)
    Q_PROPERTY(bool enableIpV4 READ enableIpV4 WRITE setEnableIpV4 NOTIFY enableIpV4Changed)
    Q_PROPERTY(bool enableIpV6 READ enableIpV6 WRITE setEnableIpV6 NOTIFY enableIpV6Changed)
    Q_PROPERTY(bool enableAutoRefresh READ enableAutoRefresh WRITE setEnableAutoRefresh NOTIFY enableAutoRefreshChanged)
public:
    explicit SAKNetworkInterfaceScanner(QObject *parent = nullptr);

private:
    QTimer *mRefreshTimer;

private:
    void refresh();

private:
    QStringList mIpList;
    QStringList ipList(){return mIpList;}
    Q_SIGNAL void ipListChanged();

    bool mEnableIpV4{true};
    bool enableIpV4(){return mEnableIpV4;}
    void setEnableIpV4(bool enable){mEnableIpV4 = enable; emit enableIpV4Changed();}
    Q_SIGNAL void enableIpV4Changed();

    bool mEnableIpV6{false};
    bool enableIpV6(){return mEnableIpV4;}
    void setEnableIpV6(bool enable){mEnableIpV6 = enable; emit enableIpV6Changed();}
    Q_SIGNAL void enableIpV6Changed();

    bool mEnableAutoRefresh{true};
    bool enableAutoRefresh(){return mEnableAutoRefresh;}
    void setEnableAutoRefresh(bool enable){mEnableAutoRefresh = enable; emit enableAutoRefreshChanged();}
    Q_SIGNAL void enableAutoRefreshChanged();
};

#endif // SAKNETWORKINTERFACESCANNER_H
