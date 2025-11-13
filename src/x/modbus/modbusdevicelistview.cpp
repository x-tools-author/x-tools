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
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include "common/iconengine.h"

#include "modbuscommon.h"
#include "modbusdevice.h"
#include "modbusdeviceeditor.h"
#include "modbusdevicelistmodel.h"
#include "modbusdevicelistmodelfilter.h"
#include "modbusregister.h"
#include "modbusregistertable.h"
#include "modbusregistertableview.h"

#define MODBUS_INVALID_DEPTH -1
#define MODBUS_DEVICE_DEPTH 0   // Modbus Device
#define MODBUS_TABLE_DEPTH 1    // Register Table
#define MODBUS_REGISTER_DEPTH 2 // Register Item

namespace xModbus {

struct ModbusDeviceListViewKeys
{
    const QString devices{"devices"};
};

ModbusDeviceListView::ModbusDeviceListView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusDeviceListView)
{
    ui->setupUi(this);
    ui->toolButtonAdd->setIcon(xIcon(":/res/icons/add.svg"));
    ui->toolButtonRemove->setIcon(xIcon(":/res/icons/remove.svg"));
    ui->toolButtonExpandAll->setIcon(xIcon(":/res/icons/expand_all.svg"));
    ui->toolButtonCollapseAll->setIcon(xIcon(":/res/icons/collapse_all.svg"));
    ui->toolButtonClose->setIcon(xColorIcon(":/res/icons/stop.svg", "#d32f2f"));
    ui->toolButtonOpen->setIcon(xColorIcon(":/res/icons/play_arrow.svg", "#388e3c"));

    m_model = new ModbusDeviceListModel(this);
    m_filter = new ModbusDeviceListModelFilter(this);
    m_filter->setSourceModel(m_model);
    ui->treeView->setModel(m_filter);
    ui->treeView->header()->hide();
#if 1
    ui->treeView->setExpandsOnDoubleClick(false);
#endif
#if 0
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
#endif

    QMenu *addMenu = new QMenu(this);
    // clang-format off
    m_addActions.device = addMenu->addAction(tr("New Modbus Device"), this, &ModbusDeviceListView::onNewDevice);
    addMenu->addSeparator();
    m_addActions.remove = addMenu->addAction(tr("Remove the Selected Item"), this, &ModbusDeviceListView::onRemove);
    // clang-format on
    ui->toolButtonAdd->setMenu(addMenu);
    ui->toolButtonAdd->setPopupMode(QToolButton::MenuButtonPopup);

    // clang-format off
    connect(addMenu, &QMenu::aboutToShow, this, &ModbusDeviceListView::onAddMenuAboutToShow);
    connect(addMenu, &QMenu::aboutToHide, this, &ModbusDeviceListView::onAddMenuAboutToHide);
    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &ModbusDeviceListView::onNewDevice);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &ModbusDeviceListView::onRemove);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &ModbusDeviceListView::onItemDoubleClicked);
    connect(ui->treeView, &QTreeView::clicked, this, &ModbusDeviceListView::onItemClicked);
    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ModbusDeviceListView::onStartButtonClicked);
    connect(ui->toolButtonClose, &QToolButton::clicked, this, &ModbusDeviceListView::onStopButtonClicked);
    connect(ui->toolButtonExpandAll, &QToolButton::clicked, this, &ModbusDeviceListView::onOpenAllItems);
    connect(ui->toolButtonCollapseAll, &QToolButton::clicked, this, &ModbusDeviceListView::onCloseAllItems);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &ModbusDeviceListView::onFilterTextChanged);
    connect(m_model, &ModbusDeviceListModel::dataModified, this, &ModbusDeviceListView::onTableDataModified);
    // clang-format on
}

ModbusDeviceListView::~ModbusDeviceListView()
{
    delete ui;
}

QJsonObject ModbusDeviceListView::save()
{
    QJsonObject obj;
    ModbusDeviceListViewKeys keys;
    QJsonArray devicesArray = m_model->save();
    obj.insert(keys.devices, devicesArray);

    return obj;
}

void ModbusDeviceListView::load(const QJsonObject &obj)
{
#if 0
    createDefaultDevices();
    return;
#endif
    ModbusDeviceListViewKeys keys;
    QJsonArray devicesArray = obj.value(keys.devices).toArray();
    if (devicesArray.isEmpty()) {
        createDefaultDevices();
    } else {
        m_model->load(devicesArray);
        ui->treeView->expandAll();
    }

    emit tableViewsUpdated();
}

QList<ModbusRegisterTableView *> ModbusDeviceListView::registerTableViews()
{
    QList<ModbusRegisterTableView *> views;
    for (int i = 0; i < m_model->rowCount(); ++i) {
        QStandardItem *deviceItem = m_model->item(i);
        for (int j = 0; j < deviceItem->rowCount(); ++j) {
            QStandardItem *tableItem = deviceItem->child(j);
            auto tableView = tableItem->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
            if (tableView) {
                views.append(tableView);
            }
        }
    }
    return views;
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
        int depth = this->treeItemDepth(index);
        if (depth == MODBUS_INVALID_DEPTH) {
            contextMenu.addAction(m_addActions.device);
        } else if (depth == MODBUS_DEVICE_DEPTH) {
        } else if (depth == MODBUS_TABLE_DEPTH) {
        }
    }

    contextMenu.exec(event->globalPos());
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
    auto item = m_model->newDevice(parameters);
    m_model->newDefaultTables(item);
    QModelIndex index = m_model->indexFromItem(item);
    ui->treeView->expand(m_filter->mapFromSource(index));
    emit tableViewsUpdated();
}

void ModbusDeviceListView::onRemove()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    const QModelIndex srcIndex = m_filter->mapToSource(index);
    QStandardItem *item = m_model->itemFromIndex(srcIndex);
    if (!item) {
        qInfo() << "ModbusDeviceListView::onRemove: item is nullptr";
        return;
    }

    int itemType = item->data(xItemTypeRole).toInt();
    if (itemType == xItemTypeDevice) {
        ModbusDevice *device = item->data(xItemTypeDevice).value<ModbusDevice *>();
        if (!device) {
            qWarning() << "ModbusDeviceListView::onRemove: device is nullptr";
        }

        if (device->isRunning()) {
            int ret = QMessageBox::question(
                this,
                tr("Remove Device"),
                tr("The device is running. Do you want to stop and remove it?"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes);

            if (ret == QMessageBox::No) {
                return;
            }
        }
    }

    m_model->removeRow(srcIndex.row(), srcIndex.parent());
    emit tableViewsUpdated();
}

void ModbusDeviceListView::onCloseAllItems()
{
    for (int i = 0; i < m_model->rowCount(); ++i) {
        QModelIndex deviceIndex = m_model->index(i, 0);
        for (int j = 0; j < m_model->rowCount(deviceIndex); ++j) {
            QModelIndex tableIndex = m_model->index(j, 0, deviceIndex);
            ui->treeView->collapse(m_filter->mapFromSource(tableIndex));
        }
    }
}

void ModbusDeviceListView::onOpenAllItems()
{
#if 0
    ui->treeView->expandToDepth(0);
#else
    ui->treeView->expandAll();
#endif
}

void ModbusDeviceListView::onItemDoubleClicked(const QModelIndex &index)
{
    QModelIndex srcIndex = m_filter->mapToSource(index);
    QStandardItem *item = m_model->itemFromIndex(srcIndex);
    if (!item) {
        qInfo() << "ModbusDeviceListView::onItemDoubleClicked: item is nullptr";
        return;
    }

    int itemTypeRole = item->data(xItemTypeRole).toInt();
    if (itemTypeRole == xItemTypeTableView) {
        QInputDialog inputDialog(xMainWindow);
        inputDialog.setWindowTitle(tr("Rename Register Table"));
        inputDialog.setLabelText(tr("Please input the new name for the register table:"));
        inputDialog.setTextValue(item->text());
        if (inputDialog.exec() == QDialog::Accepted) {
            QString newName = inputDialog.textValue();
            item->setText(newName);
            auto tableView = item->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
            if (tableView) {
                tableView->setWindowTitle(newName);
            }
        }
    } else if (itemTypeRole == xItemTypeRegister) {
        QInputDialog inputDialog(xMainWindow);
        inputDialog.setWindowTitle(tr("Rename Register Item"));
        inputDialog.setLabelText(tr("Please input the new name for the register item:"));
        inputDialog.setTextValue(item->text());
        if (inputDialog.exec() == QDialog::Accepted) {
            QString newName = inputDialog.textValue();
            item->setText(newName);
            auto registerItem = item->data(xItemTypeRegister).value<ModbusRegister *>();
            if (registerItem) {
                registerItem->setName(newName);
            }
        }
    } else if (itemTypeRole == xItemTypeDevice) {
        ModbusDeviceEditor editor(xMainWindow);
        editor.setWindowTitle(tr("Edit Modbus Device"));
        ModbusDevice *device = item->data(xItemTypeDevice).value<ModbusDevice *>();
        DeviceConnectionParameters params = device->parameters();
        QJsonObject deviceObj = deviceConnectionParameters2Json(params);
        editor.load(deviceObj);
        editor.setModal(true);
        if (editor.exec() == QDialog::Accepted) {
            QJsonObject newParams = editor.save();
            if (newParams == deviceObj) {
                return;
            }

            DeviceConnectionParameters newParamsCtx = json2DeviceConnectionParameters(newParams);
            device->setParameters(newParamsCtx);
            item->setText(newParamsCtx.deviceName);
            if (device->isRunning()) {
                int ret = QMessageBox::question(
                    this,
                    tr("Change Device parameters"),
                    tr("The device is running. Restarting it to apply new parameters now?"),
                    QMessageBox::Ok | QMessageBox::No);
                if (ret == QMessageBox::No) {
                    device->restart();
                }
            }
        }
    }
}

void ModbusDeviceListView::onItemClicked(const QModelIndex &index)
{
    QModelIndex srcIndex = m_filter->mapToSource(index);
    QStandardItem *item = m_model->itemFromIndex(srcIndex);
    if (!item) {
        qInfo() << "ModbusDeviceListView::onItemDoubleClicked: item is nullptr";
        return;
    }

    int itemTypeRole = item->data(xItemTypeRole).toInt();
    if (itemTypeRole == xItemTypeTableView) {
        QStandardItem *item = m_model->itemFromIndex(srcIndex);
        auto registerView = item->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
        if (registerView) {
            emit invokeShowRegisterView(registerView);
        }
    } else if (itemTypeRole == xItemTypeRegister) {
        QStandardItem *item = m_model->itemFromIndex(srcIndex);
        if (!item) {
            qInfo() << "ModbusDeviceListView::onItemDoubleClicked: item is nullptr";
            return;
        }

        auto registerView = item->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
        if (registerView) {
            registerView->selectRow(srcIndex.row());
            emit invokeShowRegisterView(registerView);
        }
    }
}

void ModbusDeviceListView::onAddMenuAboutToShow()
{
#if 0
    QModelIndex currentIndex = ui->treeView->currentIndex();
    int d = treeItemDepth(currentIndex);

    bool isDeviceSelected = (d == MODBUS_DEVICE_DEPTH);
    bool isRegGroupSelected = (d == MODBUS_TABLE_DEPTH);
#endif
}

void ModbusDeviceListView::onAddMenuAboutToHide()
{
    m_addActions.device->setEnabled(true);
}

void ModbusDeviceListView::onStartButtonClicked()
{
    QList<ModbusDevice *> devices = this->devices();

    for (ModbusDevice *device : devices) {
        QStandardItem *item = itemFromDevice(device);
        if (!item) {
            continue;
        }

        QList<ModbusRegister *> registers = m_model->allRegisters(device);
        device->setModbusRegisters(registers);
    }

    // Start server first
    for (ModbusDevice *device : devices) {
        if (!device->isClient()) {
            if (!device->isRunning()) {
                device->start();
            }
        }
    }

    // Start client second
    for (ModbusDevice *device : devices) {
        if (device->isClient()) {
            if (!device->isRunning()) {
                device->start();
            }
        }
    }
}

void ModbusDeviceListView::onStopButtonClicked()
{
    QList<ModbusDevice *> devices = this->devices();
    // Stop client first
    for (ModbusDevice *device : devices) {
        if (device->isClient()) {
            if (device->isRunning()) {
                device->requestInterruption();
                device->exit();
                device->wait();
            }
        }
    }

    // Stop server second
    for (ModbusDevice *device : devices) {
        if (!device->isClient()) {
            if (device->isRunning()) {
                device->requestInterruption();
                device->exit();
                device->wait();
            }
        }
    }
}

void ModbusDeviceListView::onFilterTextChanged(const QString &text)
{
    m_filter->setFilterFixedString(text);
    ui->treeView->expandAll();
}

void ModbusDeviceListView::onTableDataModified(QStandardItem *tableViewItem,
                                               const QModelIndex &index)
{
    if (index.isValid() == false) {
        qWarning() << "Table data modified: index is invalid";
        return;
    }

    auto tableView = tableViewItem->data(xItemTypeTableView).value<ModbusRegisterTableView *>();
    if (!tableView) {
        qWarning() << "Table data modified:: tableView is nullptr";
        return;
    }

    int row = index.row();
    ModbusRegisterTable *table = tableView->registerTable();
    ModbusRegister *reg = table->registerItemAt(row);
    if (!reg) {
        qWarning() << "Table data modified:: reg is nullptr";
        return;
    }

    int column = index.column();
    QStandardItem *regItem = tableViewItem->child(row, 0);
    if (!regItem) {
        qWarning() << "Table data modified:: regItem is nullptr";
        return;
    }

    if (column == REGISTER_TABLE_COLUMN_NAME) {
        regItem->setText(reg->name());
        return;
    }

    bool needToUpdateDeviceParameters = (column == REGISTER_TABLE_COLUMN_ADDRESS);
    needToUpdateDeviceParameters |= (column == REGISTER_TABLE_COLUMN_VALUE);
    needToUpdateDeviceParameters |= (column == REGISTER_TABLE_COLUMN_REGISTER_TYPE);
    if (needToUpdateDeviceParameters) {
        QStandardItem *deviceItem = tableViewItem->parent();
        if (!deviceItem) {
            qWarning() << "Table data modified:: deviceItem is nullptr";
            return;
        }

        ModbusDevice *device = deviceItem->data(xItemTypeDevice).value<ModbusDevice *>();
        if (!device) {
            qWarning() << "Table data modified:: device is nullptr";
            return;
        }

        device->setValue(reg->serverAddress(), int(reg->type()), reg->address(), reg->value());
    }
}

int ModbusDeviceListView::treeItemDepth(const QModelIndex &index)
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

QList<ModbusDevice *> ModbusDeviceListView::devices()
{
    int rowCount = m_model->rowCount();
    QList<ModbusDevice *> devices;
    for (int i = 0; i < rowCount; ++i) {
        QStandardItem *item = m_model->item(i);
        ModbusDevice *device = item->data(xItemTypeDevice).value<ModbusDevice *>();
        if (device) {
            devices.append(device);
        }
    }
    return devices;
}

QStandardItem *ModbusDeviceListView::itemFromDevice(ModbusDevice *device)
{
    int rowCount = m_model->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QStandardItem *item = m_model->item(i);
        ModbusDevice *dev = item->data(xItemTypeDevice).value<ModbusDevice *>();
        if (dev == device) {
            return item;
        }
    }
    return nullptr;
}

QList<ModbusRegister *> ModbusDeviceListView::registers(ModbusDevice *device)
{
    QList<ModbusRegister *> registers;
    QStandardItem *deviceItem = itemFromDevice(device);
    if (!deviceItem) {
        return registers;
    }

    int tableCount = deviceItem->rowCount();
    for (int i = 0; i < tableCount; ++i) {
        QStandardItem *tableItem = deviceItem->child(i);
        int registerCount = tableItem->rowCount();
        for (int j = 0; j < registerCount; ++j) {
            QStandardItem *registerItem = tableItem->child(j);
            ModbusRegister *reg = registerItem->data(xItemTypeRegister).value<ModbusRegister *>();
            if (reg) {
                registers.append(reg);
            }
        }
    }
    return registers;
}

void ModbusDeviceListView::createDefaultDevices()
{
    ModbusDeviceEditor editor(xMainWindow);

    editor.setDeviceName("TCP Client Device");
    editor.setDeviceType(static_cast<int>(XModbusType::TcpClient));
    QJsonObject parameters = editor.save();
    QStandardItem *item = m_model->newDevice(parameters);
    m_model->newDefaultTables(item);

    editor.setDeviceName("TCP Server Device");
    editor.setDeviceType(static_cast<int>(XModbusType::TcpServer));
    parameters = editor.save();
    item = m_model->newDevice(parameters);
    m_model->newDefaultTables(item);

    ui->treeView->expandAll();
}

} // namespace xModbus