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

#include "modbusdevicelistview.h"
#include "modbusregisterview.h"

namespace xModbus {

xModbus::xModbus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xModbus)
{
    ui->setupUi(this);

    connect(ui->widgetDeviceListView,
            &ModbusDeviceListView::invokeShowRegisterView,
            this,
            &xModbus::onInvokeShowRegisterView);
}

xModbus::~xModbus()
{
    delete ui;
}

void xModbus::onInvokeShowRegisterView(RegisterView* registerView)
{
    int index = ui->tabWidget->indexOf(registerView);
    if (index == -1) {
        ui->tabWidget->addTab(registerView, registerView->windowTitle());
        index = ui->tabWidget->indexOf(registerView);
    }

    ui->tabWidget->setCurrentIndex(index);
}

} // namespace xModbus
