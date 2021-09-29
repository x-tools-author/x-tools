/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKSERIALPORTTRANSPONDER_HH
#define SAKSERIALPORTTRANSPONDER_HH

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
#include "SAKDebuggerPluginTransponder.hh"

namespace Ui {
    class SAKSerialPortTransponder;
}

class SAKSerialPortTransponder : public SAKDebuggerPluginTransponder
{
    Q_OBJECT
public:
    SAKSerialPortTransponder(QWidget *parent = Q_NULLPTR);
    SAKSerialPortTransponder(
            quint64 id,
            SAKCommonDataStructure::SAKStructSerialPortParametersContext parasCtx,
            QWidget *parent = Q_NULLPTR
            );
     ~SAKSerialPortTransponder();
    QVariant parametersContext() final;
protected:
    SAKDebuggerDevice *device() final;
    void onDeviceStateChanged(bool opened) final;
private:
    Ui::SAKSerialPortTransponder *mUi;
    SAKSerialPortDevice *mDevice;
private:
    void initComponents();
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
