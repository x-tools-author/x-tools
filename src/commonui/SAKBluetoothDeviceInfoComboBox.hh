/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLUETOOTHDEVICEINFOCOMBOBOX_HH
#define SAKBLUETOOTHDEVICEINFOCOMBOBOX_HH

#include <QEvent>

#include "SAKComboBox.hh"
#include "SAKBleScanner.hh"

class SAKBluetoothDeviceInfoComboBox : public SAKComboBox
{
    Q_OBJECT
public:
    SAKBluetoothDeviceInfoComboBox(QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothDeviceInfoComboBox();
    void startDiscover();
    void stopDiscover();
    bool isActive();
    void setTimeoutInterval(int interval);
    void setNameFiltter(const QString &filtter);

signals:
    void finished();
    void started();

protected:
    virtual void changeEvent(QEvent *event) override;

private:
    SAKBleScanner *mScanner;

private slots:
    void onFinished();
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void onErrorOccurred(const QString &errStr);
};

#endif // SAKBLUETOOTHDEVICEINFOCOMBOBOX_HH
