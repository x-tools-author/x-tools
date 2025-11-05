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
#if 0
    m_toolBar = new QToolBar(this);
    m_toolBar->setWindowTitle(tr("Modbus Tool Bar"));
    m_toolBar->addAction(tr("Example Action"));
#endif

    connect(ui->widgetDeviceListView,
            &ModbusDeviceListView::invokeShowRegisterView,
            this,
            &xModbus::onInvokeShowRegisterView);
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

} // namespace xModbus
