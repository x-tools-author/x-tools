/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBLUETOOTHDEVICEINFOCOMBOBOX_H
#define SAKBLUETOOTHDEVICEINFOCOMBOBOX_H

#include <QEvent>

#include "sakblescanner.h"
#include "xToolsComboBox.h"

class SAKBluetoothDeviceInfoComboBox : public xToolsComboBox
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

#endif // SAKBLUETOOTHDEVICEINFOCOMBOBOX_H
