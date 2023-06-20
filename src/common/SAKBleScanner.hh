/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLESCANNER_HH
#define SAKBLESCANNER_HH

#include <QTimer>
#include <QObject>
#include <QVariant>
#include <QLoggingCategory>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>


class SAKBleScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList deviceInfoList READ devicesInfoList
               NOTIFY devicesInfoListChanged)
    Q_PROPERTY(bool enableRefresh READ enableRefresh WRITE setEnableRefresh
               NOTIFY enableRefreshChanged)
    Q_PROPERTY(int timeoutInterval READ timeoutInterval WRITE setTimeoutInterval
               NOTIFY timeoutIntervalChanged)
    Q_PROPERTY(QString namefiltter READ namefiltter WRITE setNameFiltter
               NOTIFY filtterNameChanged)
public:
    explicit SAKBleScanner(QObject *parent = nullptr);
    ~SAKBleScanner();

public:
    Q_INVOKABLE void startDiscover();
    Q_INVOKABLE void stopDiscover();
    Q_INVOKABLE bool isActive();

    Q_INVOKABLE QVariant deviceInfo(int index);
    Q_INVOKABLE QString deviceName(const QVariant &deviceInfo);

signals:
    void finished();

private:
    QBluetoothDeviceDiscoveryAgent *mDiscover;
    bool mIsFirstTime;
    bool mAutoRestart;
    QVector<QBluetoothDeviceInfo> mDeviceInfoListTemp;
    QLoggingCategory mLoggingCategory{"SAK.BleScanner"};

private:
    void onFinished();
    void onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//Properties
public:
    QVariantList devicesInfoList();
    bool enableRefresh();
    void setEnableRefresh(bool enable);
    int timeoutInterval();
    void setTimeoutInterval(int interval);
    QString namefiltter();
    void setNameFiltter(const QString &flag);

signals:
    void devicesInfoListChanged();
    void enableRefreshChanged();
    void timeoutIntervalChanged();
    void filtterNameChanged();

private:
    QVector<QBluetoothDeviceInfo> mDeviceInfoList;
    bool mEnableRefresh{true};
    int mTimeoutInterval{120};
    QString mNameFiltter{""};
};

#endif // SAKBLESCANNER_H
