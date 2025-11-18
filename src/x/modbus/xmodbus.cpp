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

#include <QElapsedTimer>
#include <QJsonArray>

#include "modbusdevicelistview.h"
#include "modbusregistertableview.h"
#include "utilities/iconengine.h"

namespace xModbus {

struct xModbusKeys
{
    const QString tabIndex{"tabIndex"};
    const QString listView{"listView"};
    const QString logTab{"logTab"};
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
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &xModbus::onCurrentTabChanged);
}

xModbus::~xModbus()
{
    delete ui;
}

QJsonObject xModbus::save()
{
    QJsonObject obj;
    xModbusKeys keys;
    obj.insert(keys.tabIndex, ui->tabWidget->currentIndex());
    obj.insert(keys.logTab, ui->tabLog->save());
    obj.insert(keys.listView, ui->widgetDeviceListView->save());
    return obj;
}

void xModbus::load(const QJsonObject& obj)
{
    xModbusKeys keys;
    QJsonObject logTab = obj.value(keys.logTab).toObject();
    ui->tabLog->load(logTab);
    QJsonObject listView = obj.value(keys.listView).toObject();
    ui->widgetDeviceListView->load(listView);
#if 1
    int tabIndex = obj.value(keys.tabIndex).toInt(0);
    if (tabIndex >= 0 && tabIndex < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
#endif
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
    for (int i = 0; i < tableViews.count(); ++i) {
        ModbusRegisterTableView* tableView = tableViews.at(i);
        ui->tabWidget->addTab(tableView, tableView->windowTitle());
    }
    ui->tabWidget->setCurrentWidget(currentWidget);

    if (!m_cornerToolButtonMenuActionGroup) {
        m_cornerToolButtonMenuActionGroup = new QActionGroup(this);
    }

    for (QAction* action : m_cornerToolButtonMenuActionGroup->actions()) {
        m_cornerToolButtonMenuActionGroup->removeAction(action);
    }

    m_cornerToolButtonMenu->clear();
    int count = ui->tabWidget->count();
    for (int i = 0; i < count; ++i) {
        QAction* action = m_cornerToolButtonMenu->addAction(ui->tabWidget->tabText(i));
        action->setCheckable(true);
        m_cornerToolButtonMenuActionGroup->addAction(action);

        QWidget* widget = ui->tabWidget->widget(i);
        if (widget == currentWidget) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, [this, i]() { ui->tabWidget->setCurrentIndex(i); });
        disconnect(widget, &QWidget::windowTitleChanged, this, nullptr);
        connect(widget, &QWidget::windowTitleChanged, this, [=](const QString& title) {
            ui->tabWidget->setTabText(i, title);
            action->setText(title);
        });
    }
}

void xModbus::onCurrentTabChanged()
{
    int index = ui->tabWidget->currentIndex();
    QList<QAction*> actions = m_cornerToolButtonMenuActionGroup->actions();
    if (index >= 0 && index < actions.count()) {
        actions.at(index)->setChecked(true);
    }
}

} // namespace xModbus
