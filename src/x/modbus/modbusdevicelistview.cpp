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
#include <QContextMenuEvent>
#include <QMainWindow>
#include <QMenu>

#include "common/iconengine.h"

#include "modbuscommon.h"
#include "modbusdevice.h"
#include "modbusdeviceeditor.h"
#include "modbusdevicelistmodel.h"

#define MODBUS_INVALID_DEPTH -1
#define MODBUS_DEVICE_DEPTH 0
#define MODBUS_REG_GROUP_DEPTH 1
#define MODBUS_REG_ITEM_DEPTH 2

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
#if 0
    ui->tableView->setModel(m_model);
    ui->tableView->setAcceptDrops(true);
    ui->tableView->setDropIndicatorShown(true);
    ui->tableView->setDragDropMode(QAbstractItemView::InternalMove);
#endif

    QMenu *addMenu = new QMenu(this);
    // clang-format off
    m_addActions.device = addMenu->addAction(tr("New Modbus Device"), this, &ModbusDeviceListView::onNewDevice);
    addMenu->addSeparator();
    m_addActions.coils = addMenu->addAction(tr("New Coils Group"), this, &ModbusDeviceListView::onNewCoils);
    m_addActions.discreteInputs = addMenu->addAction(tr("New Discrete Inputs Group"), this, &ModbusDeviceListView::onNewDiscreteInputs);
    m_addActions.holdingRegisters = addMenu->addAction(tr("New Holding Registers Group"), this, &ModbusDeviceListView::onNewHoldingRegisters);
    m_addActions.inputRegisters = addMenu->addAction(tr("New Input Registers Group"), this, &ModbusDeviceListView::onNewInputRegisters);
    addMenu->addSeparator();
    m_addActions.singleRegister = addMenu->addAction(tr("New single-register"), this, &ModbusDeviceListView::onNewSingleRegister);
    m_addActions.multiRegister = addMenu->addAction(tr("New multi-register"), this, &ModbusDeviceListView::onNewMultiRegister);
    addMenu->addSeparator();
    m_addActions.copy = addMenu->addAction(tr("Copy"), this, &ModbusDeviceListView::onCopy);
    m_addActions.paste = addMenu->addAction(tr("Paste"), this, &ModbusDeviceListView::onPaste);
    m_addActions.cut = addMenu->addAction(tr("Cut"), this, &ModbusDeviceListView::onCut);
    addMenu->addSeparator();
    m_addActions.undo = addMenu->addAction(tr("Undo"), this, &ModbusDeviceListView::onUndo);
    m_addActions.redo = addMenu->addAction(tr("Redo"), this, &ModbusDeviceListView::onRedo);
    addMenu->addSeparator();
    m_addActions.remove = addMenu->addAction(tr("Remove"), this, &ModbusDeviceListView::onRemove);
    // clang-format on
    ui->toolButtonAdd->setMenu(addMenu);
    ui->toolButtonAdd->setPopupMode(QToolButton::MenuButtonPopup);

    // clang-format off
    connect(addMenu, &QMenu::aboutToShow, this, &ModbusDeviceListView::onAddMenuAboutToShow);
    connect(addMenu, &QMenu::aboutToHide, this, &ModbusDeviceListView::onAddMenuAboutToHide);
    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &ModbusDeviceListView::onNewDevice);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &ModbusDeviceListView::onItemDoubleClicked);
    // clang-format on
}

ModbusDeviceListView::~ModbusDeviceListView()
{
    delete ui;
}

void ModbusDeviceListView::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = ui->treeView->mapFrom(this, event->pos());
    QMenu contextMenu(this);
    QModelIndex eventIndex = ui->treeView->indexAt(pos);
    if (!eventIndex.isValid()) {
        contextMenu.addAction(m_addActions.device);
    } else {
        QModelIndex index = ui->treeView->currentIndex();
        int depth = this->depth(index);
        if (depth == MODBUS_INVALID_DEPTH) {
            contextMenu.addAction(m_addActions.device);
        } else if (depth == MODBUS_DEVICE_DEPTH) {
            contextMenu.addAction(m_addActions.coils);
            contextMenu.addAction(m_addActions.discreteInputs);
            contextMenu.addAction(m_addActions.holdingRegisters);
            contextMenu.addAction(m_addActions.inputRegisters);
        } else if (depth == MODBUS_REG_GROUP_DEPTH) {
            contextMenu.addAction(m_addActions.singleRegister);
            contextMenu.addAction(m_addActions.multiRegister);
        }
    }

    contextMenu.exec(event->globalPos());
}

int ModbusDeviceListView::depth(const QModelIndex &index)
{
    if (!index.isValid()) {
        return MODBUS_INVALID_DEPTH;
    }

    int d = 0;
    QModelIndex parent = index.parent();
    while (parent.isValid()) {
        d++;
        parent = parent.parent();
    }
    return d;
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

void ModbusDeviceListView::onNewSingleRegister() {}

void ModbusDeviceListView::onNewMultiRegister() {}

void ModbusDeviceListView::onCopy() {}

void ModbusDeviceListView::onPaste() {}

void ModbusDeviceListView::onCut() {}

void ModbusDeviceListView::onUndo() {}

void ModbusDeviceListView::onRedo() {}

void ModbusDeviceListView::onRemove() {}

void ModbusDeviceListView::onItemDoubleClicked(const QModelIndex &index)
{
    // Nothing to do yet...
}

void ModbusDeviceListView::onAddMenuAboutToShow()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    int d = depth(currentIndex);

    bool isDeviceSelected = (d == MODBUS_DEVICE_DEPTH);
    bool isRegGroupSelected = (d == MODBUS_REG_GROUP_DEPTH);

    m_addActions.coils->setEnabled(isDeviceSelected);
    m_addActions.discreteInputs->setEnabled(isDeviceSelected);
    m_addActions.holdingRegisters->setEnabled(isDeviceSelected);
    m_addActions.inputRegisters->setEnabled(isDeviceSelected);

    m_addActions.singleRegister->setEnabled(isRegGroupSelected);
    m_addActions.multiRegister->setEnabled(isRegGroupSelected);
}

void ModbusDeviceListView::onAddMenuAboutToHide()
{
    m_addActions.device->setEnabled(true);
    m_addActions.coils->setEnabled(true);
    m_addActions.discreteInputs->setEnabled(true);
    m_addActions.holdingRegisters->setEnabled(true);
    m_addActions.inputRegisters->setEnabled(true);
    m_addActions.singleRegister->setEnabled(true);
    m_addActions.multiRegister->setEnabled(true);
}

} // namespace xModbus