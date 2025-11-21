/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "flowmeterui.h"
#include "ui_flowmeterui.h"

#include <QTimer>

#include "flowmeter.h"

FlowmeterUi::FlowmeterUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::FlowmeterUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000); // 1 second
    connect(timer, &QTimer::timeout, this, &FlowmeterUi::updateSpeedLabel);
    timer->start();
}

FlowmeterUi::~FlowmeterUi()
{
    delete ui;
}

void FlowmeterUi::addBytes(quint64 bytes)
{
    m_bytes += bytes;
}

void FlowmeterUi::updateSpeedLabel()
{
    if (m_bytes < 1024) {
        ui->labelSpeed->setText(QString::number(m_bytes) + " B/s");
    } else if (m_bytes < 1024 * 1024) {
        ui->labelSpeed->setText(QString::number(m_bytes / 1024.0, 'f', 2) + " KB/s");
    } else if (m_bytes < 1024 * 1024 * 1024) {
        ui->labelSpeed->setText(QString::number(m_bytes / (1024.0 * 1024.0), 'f', 2) + " MB/s");
    } else {
        QString txt = QString::number(m_bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB/s";
        ui->labelSpeed->setText(txt);
    }

    m_bytes = 0;
}