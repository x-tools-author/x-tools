/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTRANSPONDERSERIALPORT_HH
#define SAKDEBUGGERPLUGINTRANSPONDERSERIALPORT_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QIODevice>
#include <QPushButton>
#include <QSerialPort>
#include <QJsonObject>

#include "SAKSerialPortDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKBaseListWidgetItemWidget.hh"

namespace Ui {
    class SAKTransponderSerialPortItem;
}

class SAKTransponderSerialPortItem : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    SAKTransponderSerialPortItem(QWidget *parent = Q_NULLPTR);
    SAKTransponderSerialPortItem(
            quint64 id,
            SAKCommonDataStructure::SAKStructSerialPortParametersContext parasCtx,
            QWidget *parent = Q_NULLPTR
            );
     ~SAKTransponderSerialPortItem();
    SAKCommonDataStructure::SAKStructSerialPortParametersContext parametersContext();
private:
    Ui::SAKTransponderSerialPortItem *mUi;
    SAKSerialPortDevice *mDevice;
private:
    void initUiComponents();
    void initSignals();
signals:
    void portNameChanged(QString portName);
    void baudRateChanged(int baudRate);
    void dataBitsChanged(int baduRate);
    void parityChanged(int parity);
    void stopBitsChanged(int stopBits);
    void flowControlChanged(int flowControl);
    void frameIntervalChanged(int frameInterval);

};

#endif
