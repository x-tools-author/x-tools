/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class ModbusDeviceListView;
}

namespace xModbus {

class ModbusDevice;
class ModbusDeviceListModel;
class ModbusDeviceListView : public QWidget
{
    Q_OBJECT
public:
    explicit ModbusDeviceListView(QWidget *parent = nullptr);
    ~ModbusDeviceListView() override;

signals:
    void currentDeviceChanged(ModbusDevice *device);

private:
    void onNewDevice();
    void onNewRtuDevice();
    void onNewTcpDevice();
    void onNewCoils();
    void onNewDiscreteInputs();
    void onNewHoldingRegisters();
    void onNewInputRegisters();

private:
    Ui::ModbusDeviceListView *ui;
    ModbusDeviceListModel *m_model;
};

} // namespace xModbus