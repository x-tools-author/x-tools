/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusdevicelistview.h"
#include "ui_modbusdevicelistview.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>

#include "common/iconengine.h"

#include "modbuscommon.h"
#include "modbusdevice.h"
#include "modbusdeviceeditor.h"
#include "modbusdevicelistmodel.h"

namespace xModbus {

ModbusDeviceListView::ModbusDeviceListView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusDeviceListView)
{
    ui->setupUi(this);
    ui->toolButtonAdd->setIcon(xIcon(":/res/icons/add.svg"));
    ui->toolButtonRemove->setIcon(xIcon(":/res/icons/remove.svg"));
    ui->toolButtonTree->setIcon(xIcon(":/res/icons/account_tree.svg"));
    ui->toolButtonUp->setIcon(xIcon(":/res/icons/arrow_upward.svg"));
    ui->toolButtonDown->setIcon(xIcon(":/res/icons/arrow_downward.svg"));
    ui->toolButtonClose->setIcon(xIcon(":/res/icons/stop.svg"));
    ui->toolButtonOpen->setIcon(xIcon(":/res/icons/play_arrow.svg"));

    m_model = new ModbusDeviceListModel(ui->treeView);
    ui->treeView->setModel(m_model);
    ui->treeView->header()->hide();

    QMenu *addMenu = new QMenu(this);
    addMenu->addAction(tr("New RTU Device"), this, &ModbusDeviceListView::onNewRtuDevice);
    addMenu->addAction(tr("New TCP Device"), this, &ModbusDeviceListView::onNewTcpDevice);
    addMenu->addSeparator();
    addMenu->addAction(tr("New Coils"), this, &ModbusDeviceListView::onNewCoils);
    addMenu->addAction(tr("New Discrete Inputs"), this, &ModbusDeviceListView::onNewDiscreteInputs);
    addMenu->addAction(tr("New Holding Registers"),
                       this,
                       &ModbusDeviceListView::onNewHoldingRegisters);
    addMenu->addAction(tr("New Input Registers"), this, &ModbusDeviceListView::onNewInputRegisters);
    ui->toolButtonAdd->setMenu(addMenu);
    ui->toolButtonAdd->setPopupMode(QToolButton::MenuButtonPopup);
    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &ModbusDeviceListView::onNewDevice);
}

ModbusDeviceListView::~ModbusDeviceListView()
{
    delete ui;
}

void ModbusDeviceListView::onNewDevice()
{
    ModbusDeviceEditor editor(xMainWindow);
    editor.setModal(true);
    int ret = editor.exec();
    if (ret != QDialog::Accepted) {
        return;
    }

    QJsonObject parameters = editor.save();
    ModbusDevice *device = new ModbusDevice(parameters, this);
    m_model->addDevice(device);
}

void ModbusDeviceListView::onNewRtuDevice()
{
    // Logic to create a new RTU device, e.g., open a dialog to configure the RTU device.
}

void ModbusDeviceListView::onNewTcpDevice()
{
    // Logic to create a new TCP device, e.g., open a dialog to configure the TCP device.
}

void ModbusDeviceListView::onNewCoils()
{
    // Logic to create a new Coils device, e.g., open a dialog to configure the Coils.
}

void ModbusDeviceListView::onNewDiscreteInputs()
{
    // Logic to create a new Discrete Inputs device, e.g., open a dialog to configure the Discrete Inputs.
}

void ModbusDeviceListView::onNewHoldingRegisters()
{
    // Logic to create a new Holding Registers device, e.g., open a dialog to configure the Holding Registers.
}

void ModbusDeviceListView::onNewInputRegisters()
{
    // Logic to create a new Input Registers device, e.g., open a dialog to configure the Input Registers.
}

} // namespace xModbus