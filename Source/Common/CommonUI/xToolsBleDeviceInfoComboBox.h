/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QEvent>

#include "xToolsBleScanner.h"
#include "xToolsComboBox.h"

class xToolsBleDeviceInfoComboBox : public xToolsComboBox
{
    Q_OBJECT
public:
    xToolsBleDeviceInfoComboBox(QWidget *parent = Q_NULLPTR);
    ~xToolsBleDeviceInfoComboBox();
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
    xToolsBleScanner *m_scanner;

private slots:
    void onFinished();
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void onErrorOccurred(const QString &errStr);
};
