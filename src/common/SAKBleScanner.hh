/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef SAKBLESCANNER_HH
#define SAKBLESCANNER_HH

#include <QTimer>
#include <QObject>
#include <QVariant>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>


class SAKBleScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList names READ names NOTIFY namesChanged)
    Q_PROPERTY(bool isDiscovering READ isDiscovering NOTIFY isDiscoveringChanged)
    Q_PROPERTY(bool enableRefresh READ enableRefresh WRITE setEnableRefresh NOTIFY enableRefreshChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(QString filtter READ filtter WRITE setFiltter NOTIFY filtterChanged)
public:
    explicit SAKBleScanner(QObject *parent = nullptr);
    ~SAKBleScanner();

    Q_INVOKABLE QVariant bleInfo(int index);

private:
    QBluetoothDeviceDiscoveryAgent *mDiscover;
    QTimer *mTimer{nullptr};

private:
    void refresh();
    void onFinished();
    void onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);

private:
    QStringList mNames;
    QStringList names(){return mNames;}
    Q_SIGNAL void namesChanged();

    bool mIsDiscovering{false};
    bool isDiscovering(){return mIsDiscovering;}
    Q_SIGNAL void isDiscoveringChanged();

    bool mEnableRefresh{true};
    bool enableRefresh(){return mEnableRefresh;}
    void setEnableRefresh(bool enable){mEnableRefresh = enable; emit enableRefreshChanged();}
    Q_SIGNAL void enableRefreshChanged();

    int mTimeout{10};
    int timeout(){return mTimeout;}
    void setTimeout(int interval){mTimeout = interval; emit timeoutChanged();}
    Q_SIGNAL void timeoutChanged();

    QString mFiltter;
    QString filtter(){return mFiltter;}
    void setFiltter(const QString &flag){mFiltter = flag; emit filtterChanged();}
    Q_SIGNAL void filtterChanged();
};

#endif // SAKBLESCANNER_H
