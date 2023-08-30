/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QModbusClient>
#include <QModbusServer>

#include "sakmodbusregisterview.h"
#include "ui_sakmodbusregisterview.h"

SAKModbusRegisterView::SAKModbusRegisterView(QWidget *parent)
    : QWidget{parent}
    , ui_(new Ui::SAKModbusRegisterView)
    , modbus_device_(Q_NULLPTR)
    , modbus_client_(Q_NULLPTR)
    , modbus_server_(Q_NULLPTR){
    ui_->setupUi(this);

    QStringList labels;
    labels << tr("Address") << tr("Value") << tr("Description");
    ui_->register_table_widget_->setColumnCount(3);
    ui_->register_table_widget_->setHorizontalHeaderLabels(labels);
    QHeaderView *h_view = ui_->register_table_widget_->horizontalHeader();
    h_view->setStretchLastSection(true);
    QHeaderView *v_view = ui_->register_table_widget_->verticalHeader();
    v_view->hide();

    ui_->auto_send_combo_box_->addItem(tr("Disable"), -1);
    for (int i = 100; i <= 1000; i+= 100) {
        ui_->auto_send_combo_box_->addItem(QString::number(i), i);
    }
    for (int i = 2000; i <= 10000; i+= 200) {
        ui_->auto_send_combo_box_->addItem(QString::number(i), i);
    }

    ui_->format_combo_box_->addItem("Bin");
    ui_->format_combo_box_->addItem("Dec");
    ui_->format_combo_box_->addItem("Hex");

    SetUiEnable(false);

    auto_send_timer_ = new QTimer(this);
    auto_send_timer_->setSingleShot(true);
    connect(auto_send_timer_, &QTimer::timeout,
            this, &SAKModbusRegisterView::OnAutoSendTimerTimtout);

    connect(ui_->auto_send_combo_box_, &QComboBox::activated,
            this, &SAKModbusRegisterView::OnAutoSendComboBoxActived);
    connect(ui_->send_push_button_, &QPushButton::clicked,
            this, &SAKModbusRegisterView::OnSendPushButtonClicked);
    connect(ui_->address_spin_box_, &QSpinBox::valueChanged,
            this, &SAKModbusRegisterView::OnAddressChanged);
    connect(ui_->quantity_spin_box_, &QSpinBox::valueChanged,
            this, &SAKModbusRegisterView::OnQuantityChanged);
}

SAKModbusRegisterView::~SAKModbusRegisterView() {
    delete ui_;
}

void SAKModbusRegisterView::ResetModbusDevice(QModbusDevice *device) {
    modbus_device_ = device;
    modbus_client_ = qobject_cast<QModbusClient*>(device);
    modbus_server_ = qobject_cast<QModbusServer*>(device);
    SetUiEnable(modbus_device_);

    if (modbus_device_ && modbus_device_->inherits("QModbusClient")) {
        ResetClient(device);
    } else if (modbus_device_ && modbus_device_->inherits("QModbusServer")) {
        ResetServer(device);
    } else {
        qWarning(logging_category_) << "unknown device type!";
    }
}

void SAKModbusRegisterView::SetUiVisible(bool is_client) {
    ui_->function_label_->setVisible(is_client);
    ui_->function_combo_box_->setVisible(is_client);
    ui_->auto_send_label_->setVisible(is_client);
    ui_->auto_send_combo_box_->setVisible(is_client);
    ui_->send_push_button_->setVisible(is_client);
}

void SAKModbusRegisterView::SetUiEnable(bool enable) {
    ui_->auto_send_combo_box_->setEnabled(enable);
    ui_->send_push_button_->setEnabled(enable);
}

void SAKModbusRegisterView::ResetServer(QModbusDevice *device) {
    SetUiVisible(false);
    modbus_server_ = qobject_cast<QModbusServer*>(device);
}

void SAKModbusRegisterView::ResetClient(QModbusDevice *device) {
    SetUiVisible(true);
    modbus_client_ = qobject_cast<QModbusClient*>(device);
}

void SAKModbusRegisterView::SendRequest() {
    if (!modbus_client_) {
        qWarning() << "unsupported operation, ignored the operation.";
        return;
    }
}

QList<quint16> SAKModbusRegisterView::GetValues() {
    QList<quint16> values;
    QTableWidget *table_widget = ui_->register_table_widget_;
    for (int i = 0; i < table_widget->rowCount(); i++) {
        QTableWidgetItem *item = table_widget->item(i, 1);
        if (item) {
            values.append(item->text().toInt());
        } else {
            values.append(0);
        }
    }

    return values;
}

void SAKModbusRegisterView::SetValues(const QList<quint16> &values) {
    QTableWidget *table_widget = ui_->register_table_widget_;
    for (int i = 0; i < table_widget->rowCount(); i++) {
        QTableWidgetItem *item = table_widget->item(i, 1);
        QString txt = "-";
        if (i >= 0 && i < values.length()) {
            txt = QString::number(values.at(i));
        }
        if (item) {
            item->setText(txt);
        } else {
            item = new QTableWidgetItem();
            item->setText(txt);
            table_widget->setItem(i, 1, item);
        }
    }
}

void SAKModbusRegisterView::UpdateTableWidget()
{

}

void SAKModbusRegisterView::OnAutoSendComboBoxActived() {
    int interval = ui_->auto_send_combo_box_->currentData().toInt();
    if (interval < 100) {
        auto_send_timer_->stop();
    }

    auto_send_timer_->setInterval(interval);
    auto_send_timer_->start();
}

void SAKModbusRegisterView::OnAutoSendTimerTimtout() {
    SendRequest();
    auto_send_timer_->start();
}

void SAKModbusRegisterView::OnSendPushButtonClicked() {
    SendRequest();
}

void SAKModbusRegisterView::OnAddressChanged(int address) {
    Q_UNUSED(address)
}

void SAKModbusRegisterView::OnQuantityChanged(int quantity) {
    Q_UNUSED(quantity)
}
