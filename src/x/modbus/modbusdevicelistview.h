/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QStandardItem>
#include <QWidget>

namespace Ui {
class ModbusDeviceListView;
}

namespace xModbus {

class ModbusDevice;
class ModbusRegister;
class ModbusDeviceListModel;
class ModbusRegisterTableView;
class ModbusDeviceListModelFilter;
class ModbusDeviceListView : public QWidget
{
    Q_OBJECT
public:
    explicit ModbusDeviceListView(QWidget *parent = nullptr);
    ~ModbusDeviceListView() override;

    QJsonObject save();
    void load(const QJsonObject &obj);
    QList<ModbusRegisterTableView *> registerTableViews();

signals:
    void currentDeviceChanged(ModbusDevice *device);
    void tableViewsUpdated();
    void invokeShowRegisterView(ModbusRegisterTableView *view);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    struct
    {
        QAction *device;
        QAction *remove;
    } m_addActions;

private:
    Ui::ModbusDeviceListView *ui;
    ModbusDeviceListModel *m_model;
    ModbusDeviceListModelFilter *m_filter;

private:
    void onNewDevice();
    void onRemove();
    void onCloseAllItems();
    void onOpenAllItems();
    void onItemDoubleClicked(const QModelIndex &index);
    void onItemClicked(const QModelIndex &index);
    void onAddMenuAboutToShow();
    void onAddMenuAboutToHide();
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onFilterTextChanged(const QString &text);

    int treeItemDepth(const QModelIndex &index);
    QList<ModbusDevice *> devices();
    QStandardItem *itemFromDevice(ModbusDevice *device);
    QList<ModbusRegister *> registers(ModbusDevice *device);
    void createDefaultDevices();
};

} // namespace xModbus