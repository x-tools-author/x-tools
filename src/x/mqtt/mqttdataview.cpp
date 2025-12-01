/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttdataview.h"
#include "ui_mqttdataview.h"

#include "mqttdatafilter.h"
#include "mqttdatamodel.h"
#include "utilities/iconengine.h"

namespace xMQTT {

struct MqttDataViewParameterKeys
{
    const QString pingPongMessagesVisible{"pingPongMessagesVisible"};
    const QString autoScrollingEnabled{"autoScrollingEnabled"};
    const QString filterText{"filterText"};
};

MqttDataView::MqttDataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttDataView)
{
    ui->setupUi(this);
    m_model = new MqttDataModel(this);
    m_filter = new MqttDataFilter(this);
    m_filter->setSourceModel(m_model);
    ui->tableView->setModel(m_filter);
    ui->tableView->setAlternatingRowColors(true);
    QHeaderView *hView = ui->tableView->horizontalHeader();
    hView->setSectionResizeMode(QHeaderView::ResizeToContents);
    hView->setStretchLastSection(true);
    hView->setMinimumSectionSize(80);
    ui->toolButtonClear->setIcon(xIcon(":/res/icons/mop.svg"));
    ui->toolButtonPing->setIcon(xIcon(":/res/icons/bigtop_updates.svg"));
    ui->toolButtonScrolling->setIcon(xIcon(":/res/icons/low_priority.svg"));
    ui->toolButtonPing->setCheckable(true);
    ui->toolButtonPing->setChecked(true);
    ui->toolButtonScrolling->setCheckable(true);
    ui->toolButtonScrolling->setChecked(true);
    connect(ui->toolButtonClear, &QToolButton::clicked, m_model, &MqttDataModel::clear);
    connect(m_model, &MqttDataModel::rowsInserted, this, &MqttDataView::onRowInserted);
    connect(ui->toolButtonPing, &QToolButton::toggled, this, &MqttDataView::onPingPongToggled);
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &MqttDataView::onFilterTextChanged);
}

MqttDataView::~MqttDataView()
{
    delete ui;
}

QJsonObject MqttDataView::save()
{
    QJsonObject obj;
    const MqttDataViewParameterKeys keys;
    obj.insert(keys.pingPongMessagesVisible, ui->toolButtonPing->isChecked());
    obj.insert(keys.autoScrollingEnabled, ui->toolButtonScrolling->isChecked());
    obj.insert(keys.filterText, ui->lineEditFilter->text());
    return obj;
}

void MqttDataView::load(const QJsonObject &obj)
{
    const MqttDataViewParameterKeys keys;
    ui->toolButtonPing->setChecked(obj.value(keys.pingPongMessagesVisible).toBool(true));
    ui->toolButtonScrolling->setChecked(obj.value(keys.autoScrollingEnabled).toBool(true));
    ui->lineEditFilter->setText(obj.value(keys.filterText).toString());

    m_filter->setPingPongMessagesVisible(ui->toolButtonPing->isChecked());
    m_filter->setFilterFixedString(ui->lineEditFilter->text());
}

MqttDataModel *MqttDataView::model() const
{
    return m_model;
}

void MqttDataView::onRowInserted()
{
    if (ui->toolButtonScrolling->isChecked()) {
        ui->tableView->scrollToBottom();
    }
}

void MqttDataView::onPingPongToggled(bool checked)
{
    m_filter->setPingPongMessagesVisible(checked);
}

void MqttDataView::onFilterTextChanged(const QString &text)
{
    m_filter->setFilterFixedString(text);
}

} // namespace xMQTT