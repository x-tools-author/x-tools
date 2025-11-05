/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xmodbus.h"
#include "ui_xmodbus.h"

#include <QJsonArray>

#include "common/iconengine.h"
#include "modbusdevicelistview.h"
#include "modbusregistertableview.h"

namespace xModbus {

struct xModbusKeys
{
    const QString listView{"listView"};
};

xModbus::xModbus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xModbus)
{
    ui->setupUi(this);
    m_cornerToolButton = new QToolButton(this);
    m_cornerToolButton->setIcon(xIcon(":/res/icons/list.svg"));
    ui->tabWidget->setCornerWidget(m_cornerToolButton, Qt::TopRightCorner);
    m_cornerToolButtonMenu = new QMenu(this);
    m_cornerToolButton->setMenu(m_cornerToolButtonMenu);
    m_cornerToolButton->setPopupMode(QToolButton::InstantPopup);
#if 0
    m_toolBar = new QToolBar(this);
    m_toolBar->setWindowTitle(tr("Modbus Tool Bar"));
    m_toolBar->addAction(tr("Example Action"));
#endif

    connect(ui->widgetDeviceListView,
            &ModbusDeviceListView::invokeShowRegisterView,
            this,
            &xModbus::onInvokeShowRegisterView);
    connect(ui->widgetDeviceListView,
            &ModbusDeviceListView::tableViewsUpdated,
            this,
            &xModbus::onTableViewsUpdated);
}

xModbus::~xModbus()
{
    delete ui;
}

QJsonObject xModbus::save()
{
    QJsonObject obj;

    return obj;
}

void xModbus::load(const QJsonObject& obj)
{
    xModbusKeys keys;
    QJsonObject listView = obj.value(keys.listView).toObject();
    ui->widgetDeviceListView->load(listView);
}

void xModbus::onInvokeShowRegisterView(ModbusRegisterTableView* registerView)
{
    int index = ui->tabWidget->indexOf(registerView);
    if (index == -1) {
        ui->tabWidget->addTab(registerView, registerView->windowTitle());
        index = ui->tabWidget->indexOf(registerView);
    }

    ui->tabWidget->setCurrentIndex(index);
}

void xModbus::onTableViewsUpdated()
{
    QWidget* currentWidget = ui->tabWidget->currentWidget();
    while (ui->tabWidget->count() > 1) {
        ui->tabWidget->removeTab(1);
    }

    auto tableViews = ui->widgetDeviceListView->registerTableViews();
    for (ModbusRegisterTableView* tableView : tableViews) {
        ui->tabWidget->addTab(tableView, tableView->windowTitle());
    }
    ui->tabWidget->setCurrentWidget(currentWidget);

    m_cornerToolButtonMenu->clear();
    int count = ui->tabWidget->count();
    for (int i = 0; i < count; ++i) {
        QAction* action = m_cornerToolButtonMenu->addAction(ui->tabWidget->tabText(i));
        connect(action, &QAction::triggered, [this, i]() { ui->tabWidget->setCurrentIndex(i); });
    }
}

} // namespace xModbus
